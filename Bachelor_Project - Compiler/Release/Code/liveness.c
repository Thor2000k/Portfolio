#include "liveness.h"
#include "intermediate_tree.h"
#include "imt_print.h"
#include <stdio.h>
#include <stdint.h>

hash_table_t *label_lookup;
statement_block_t *header;
flow_node_t *previous_node;
hash_table_t *jmp_queue;

// local declaration
static void live_argument(flow_node_t *node, arg_t *arg, int is_def, int is_addr);

static flow_node_t *live_init_flow_node(entry_t *entry)
{
    flow_node_t *node = NEW(flow_node_t);
    node->entry = entry;
    node->defined = AL_INIT_PTR(2);
    node->used = AL_INIT_PTR(4);
    node->def_row = 0UL;
    node->use_row = 0UL;

    node->basic_succ = NULL;
    node->jmp_succ = NULL;
    node->next = NULL;
    node->previous = NULL;

    return node;
}

static flow_node_t *live_add_flow_node(statement_block_t *block, entry_t *entry)
{
    flow_node_t *node = live_init_flow_node(entry);
    node->previous = previous_node;

    if (previous_node != NULL)
        previous_node->next = node;

    if (block->start == NULL)
        block->start = node;
    
    block->last = node;
    block->size += 1;

    return node;
}

static statement_block_t *live_init_statement_block()
{
    statement_block_t *block = NEW(statement_block_t);

    block->next = NULL;
    block->temps = ht_init_hash_table(kind_hash_pointer, kind_ht_key_pointer);
    block->variables = ht_init_hash_table(kind_hash_int, kind_ht_key_pointer);
    block->size = 0;

    block->start = NULL;
    block->last = NULL;
    block->next = NULL;
    block->temp_in_table = NULL;

    return block;
}

// Allocate resources for a row
static void live_block_row(hash_table_t *temps, uint64_t in_row,
 uint64_t *live_temps, uint64_t *used_regs, int *temp_assignment)
{
    arg_t *arg;
    unsigned int is_live;
    unsigned int columns = temps->size;
    
    // reduce number of dereferences
    uint64_t l_live_temps = *live_temps;
    uint64_t l_used_regs = *used_regs;

    for (uint64_t j = 0U; j < columns; j++)
    {
        // check if temp j is live
        is_live = in_row & 1UL;

        // temp j is assigned and not live
        if (!is_live && temp_assignment[j] != -1)
        {
            REMOVE(l_used_regs, temp_assignment[j]);

            temp_assignment[j] = -1;
            REMOVE(l_live_temps, j);
        } // temp j is live and not assigned
        else if (is_live && !GET(l_live_temps, j))
        {
            SET(l_live_temps, j);
            
            arg = ht_get_pointer(temps, (void *)j, NULL);
            arg->kind = kind_arg_register;
            
            int s;
            for (s = 0; s < 5 && GET(l_used_regs, s); s++);

            if (s == 5)
            {
                fprintf(stderr, "ERROR: Ran out of registers.\n");
                abort();
            }

            temp_assignment[j] = s;
            SET(l_used_regs, s);
            
            arg->value.reg = asm_callee_save[s];
        }

        in_row = in_row >> 1;
    }

    *live_temps = l_live_temps;
    *used_regs = l_used_regs;
}

// Allocate resources for a statement block
static void live_block_assignment(statement_block_t *block)
{
    uint64_t rows = block->size;
    uint64_t columns = block->temps->size;
    int *temp_assignment = NULL;

    if (columns > 0)
        temp_assignment = NEWARRAY(int, columns);

    for (unsigned int i = 0; i < columns; i++)
        temp_assignment[i] = -1;

    uint64_t used_regs = 0U;
    uint64_t live_temps = 0U;

    for (uint64_t i = 0; i < rows; i++)
    {
        live_block_row(block->temps, block->temp_in_table[i], &live_temps, 
            &used_regs, temp_assignment);
    }

    if (temp_assignment != NULL)
        free(temp_assignment);
}

// Remove liveness when a function is called
static int live_block_calls(statement_block_t *block, unsigned int i,
 uint64_t ignore, flow_node_t **node)
{
    unsigned int rows = block->size;
    flow_node_t *current_node = *node;
    entry_t *entry;
    uint64_t in_row;
    for (; i < rows; i++, current_node = current_node->next)
    {
        entry = current_node->entry;
        in_row = DIFF(block->temp_in_table[i], ignore);

        if (entry->kind == kind_ent_tag)
        {
            if (entry->value.tag == kind_tag_func_start)
            {
                unsigned int columns = block->temps->size;
                array_list *live_temps = AL_INIT_PTR(5);
                for (uint64_t j = 0; j < columns; j++)
                {
                    if (GET(in_row, j))
                    {
                        arg_t *arg = ht_get_pointer(block->temps, (void *)j, NULL);
                        al_add(live_temps, &arg);
                    }
                }

                entry->extra_data = live_temps;

                current_node = current_node->next;
                i = live_block_calls(block, i + 1, in_row, &current_node);
                
                entry = current_node->entry;
                in_row = DIFF(block->temp_in_table[i], ignore);

                entry->extra_data = live_temps;
            }
            else
            {
                *node = current_node;
                return i;
            }
        }

        block->temp_in_table[i] = in_row;
    }

    *node = current_node;
    return i;
}

// Set liveness for a statement block
static void live_block_analysis(statement_block_t *block)
{
    int rows = block->size;
    uint64_t *in = NULL;
    if (rows > 0)
        in = NEWARRAY(uint64_t, rows);

    arg_t *temp;
    int i;
    uint64_t use_row, def_row;

    flow_node_t *node = block->last;
    uint64_t out_row = 0UL;
    for (int j = rows - 1; j >= 0; j--, node = node->previous)
    {
        use_row = 0UL;
        def_row = 0UL;

        AL_FOR_EACH(arg_t *, temp, node->used, i)
            SET(use_row, temp->value.temporary); 

        AL_FOR_EACH(arg_t *, temp, node->defined, i)
            SET(def_row, temp->value.temporary);

        // next out row == this in row
        out_row = UNION(use_row, DIFF(out_row, def_row));
        in[j] = out_row;
    }

    block->temp_in_table = in;    

    flow_node_t *start_node = block->start;
    live_block_calls(block, 0U, 0UL, &start_node);
    
    node = block->start;
    int columns = block->temps->size;
    
    if (debug_show_liveness)
    {
        fprintf(stderr, "\nBlock start\n");
        for (i = 0; i < rows && columns > 0; i++, node = node->next)
        {
            use_row = in[i];
            for (int j = 0; j < columns; j++)
                fprintf(stderr, "%ld", GET(use_row, j));

            fprintf(stderr, "  ");
            imt_print_text_section(node->entry);
        }
    }

    live_block_assignment(block);

    if (in != NULL) 
        free(in);
}

static void live_address(flow_node_t *node, address_t *address, int is_def)
{
    switch (address->kind)
    {
        case kind_addr_label:
            ht_put_pointer(jmp_queue, node, address->value.label);
            break;
    
        case kind_addr_dereference:
            live_argument(node, address->value.deref_address, is_def, 1);
            break;

        case kind_addr_offset_derefference:
            live_argument(node, address->value.deref_offset.deref_address, is_def, 1);
            break;

        case kind_addr_complex_derefference:
            live_argument(node, address->value.deref_complex.base, is_def, 1);
            live_argument(node, address->value.deref_complex.index, is_def, 1);
            break;            
    }
}

static void live_argument(flow_node_t *node, arg_t *arg, int is_def, int is_addr)
{
    switch (arg->kind)
    {
        case kind_arg_constant:
        case kind_arg_register:
            break;

        case kind_arg_address:
            live_address(node, arg->value.address, is_def);
            break;    
        
        case kind_arg_temporary:
            if (is_def && !is_addr)
                al_add(node->defined, &arg);
            else
                al_add(node->used, &arg);

            break;
    }
}

static void live_instruction(flow_node_t *node, ins_t *ins)
{
    if (ins->kind != kind_ins_jump && previous_node != NULL)
        previous_node->basic_succ = node;

    switch (ins->kind)
    {
        // binary
        case kind_ins_cond_jump:
            live_argument(node, ins->input.cond_jump.src, 0, 0);
            live_argument(node, ins->input.cond_jump.dst, 0, 0);
            break;

        case kind_ins_add:
        case kind_ins_sub:
        case kind_ins_mult:
        case kind_ins_divide:
        case kind_ins_greater:
        case kind_ins_greater_eq:
        case kind_ins_lower:
        case kind_ins_lower_eq:
        case kind_ins_equal:
        case kind_ins_not_equal:
        case kind_ins_cond_assign:
        case kind_ins_modulo:
            live_argument(node, ins->input.bin_args.src, 0, 0);
            live_argument(node, ins->input.bin_args.dst, 0, 0);

            break;
        case kind_ins_assign:
        case kind_ins_lea:
            live_argument(node, ins->input.bin_args.src, 0, 0);
            live_argument(node, ins->input.bin_args.dst, 1, 0);
            break;

        // unary
        case kind_ins_jump:
        case kind_ins_increment:
        case kind_ins_decrement:
        case kind_ins_push:
        case kind_ins_pop:
        case kind_ins_not:
        case kind_ins_neg:
        case kind_ins_call:
        case kind_ins_arithmetic_lshift:
            live_argument(node, ins->input.unary_arg, 0, 0);
            break;
    }
}

/* check an entry for the block, returns 1 on finding a statement tag. */
static int live_entry(statement_block_t *block, entry_t *entry)
{
    flow_node_t *node;
    arg_t *arg;
    int i;
    switch (entry->kind)
    {
        case kind_ent_comment:
        case kind_ent_empty:
            break;

        case kind_ent_label:
            node = live_add_flow_node(block, entry);
            ht_put_pointer(label_lookup, entry->value.label, node);
            previous_node = node;
            break;

        case kind_ent_instruction:
            node = live_add_flow_node(block, entry);
            live_instruction(node, entry->value.instruction);
            previous_node = node;

            AL_FOR_EACH(arg_t *, arg, node->used, i)
                ht_put_pointer(block->temps, (void *)arg->value.temporary, arg);

            AL_FOR_EACH(arg_t *, arg, node->defined, i)
                ht_put_pointer(block->temps, (void *)arg->value.temporary, arg);

            break;    

        case kind_ent_tag:
            if (entry->value.tag == kind_tag_statement_start)
                return 1;
            else
                previous_node = live_add_flow_node(block, entry);
            
            break;
    }

    return 0;
}

static void live_text_section(array_list *entry_list)
{
    int i;
    entry_t *entry;

    previous_node = NULL;
    header = live_init_statement_block();
    label_lookup = ht_init_hash_table(kind_hash_pointer, kind_ht_key_string);
    jmp_queue = ht_init_hash_table(kind_hash_pointer, kind_ht_key_pointer);
    statement_block_t *current_block = header;
    
    // create flow-control graph
    AL_FOR_EACH(entry_t *, entry, entry_list, i)
    {
        if (live_entry(current_block, entry))
        {
            current_block->last = previous_node;
            current_block->next = live_init_statement_block();
            current_block = current_block->next;
        }
    }

    flow_node_t *node;
    char *label;
    array_list *keys = ht_get_keys(jmp_queue);
    AL_FOR_EACH(flow_node_t *, node, keys, i)
    {
        label = ht_get_pointer(jmp_queue, node, NULL);
        node->jmp_succ = ht_get_pointer(label_lookup, label, NULL);
    }

    al_destroy(keys, NULL);
    ht_destroy(label_lookup);
    ht_destroy(jmp_queue);

    current_block = header;
    while (current_block != NULL)
    {
        live_block_analysis(current_block);
        current_block = current_block->next;
    }
}

array_list *live_start_analysis(program_t *program)
{
    array_list *block_list = AL_INIT_PTR(10);
    
    live_text_section(program->text_section);
    al_add(block_list, &header);

    function_section_t *function;
    int i;

    AL_FOR_EACH(function_section_t *, function, program->functions, i)
    {
        live_text_section(function->body);
        al_add(block_list, &header);
    }

    return block_list;
}
