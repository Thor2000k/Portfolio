#include "intermediate_tree.h"
#include "memory.h"
#include <stdio.h>

int entry_count = 0;

program_t *imt_make_program()
{
    program_t *program = NEW(program_t);

    program->data_section = AL_INIT_PTR(10);
    program->text_section = AL_INIT_PTR(10);
    program->functions = AL_INIT_PTR(10);

    return program;
}

function_section_t *imt_make_function(function_t *asm_node, char *name, 
    char *end_label, int offset_table_start)
{
    function_section_t *function = NEW(function_section_t);

    function->offset_table_start = offset_table_start;
    function->asm_node = asm_node;
    function->name = name;
    function->end_label = end_label;
    function->body = AL_INIT_PTR(10);

    return function;
}

data_decl_t *imt_make_data_decl(char *label, int expected_count, 
    imt_data_kind_t kind)
{
    data_decl_t *decl = NEW(data_decl_t);

    decl->label = label;
    decl->kind = kind;

    switch (kind)
    {
        case kind_data_byte:
            decl->values = al_init_list(expected_count, sizeof(int8_t));
            break;

        case kind_data_int:
            decl->values = al_init_list(expected_count, sizeof(int32_t));
            break;

        case kind_data_pointer:
            decl->values = al_init_list(expected_count, sizeof(int64_t));
            break;
    }

    return decl;
}

entry_t *imt_make_entry(void *param, imt_entry_kind_t kind)
{
    entry_t *entry = NEW(entry_t);
    entry->id = entry_count;
    entry->kind = kind;
    entry->extra_data = NULL;
    
    switch (kind)
    {
        case kind_ent_instruction:
            entry->value.instruction = param;
            break;

        case kind_ent_label:
            entry->value.label = param;
            break;

        case kind_ent_comment:
            entry->value.comment = param;
            break;

        case kind_ent_empty:
        case kind_ent_tag:
            entry->value.tag = *(imt_tag_kind_t *)param;            
            break;
    }

    entry_count++;
    return entry;
}

ins_t *imt_make_instruction(arg_t *src, arg_t *dst, char *label,
 imt_ins_kind_t kind, imt_ins_size_t size)
{
    ins_t *ins = NEW(ins_t);
    ins->kind = kind;
    ins->size = size;

    switch (kind)
    {
        case kind_ins_cond_jump:
            ins->input.cond_jump.src = src;
            ins->input.cond_jump.dst = dst;
            ins->input.cond_jump.label = label;
            break;
            
        // binary
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
        case kind_ins_assign:
        case kind_ins_cond_assign:
        case kind_ins_lea:
        case kind_ins_modulo:
            ins->input.bin_args.src = src;
            ins->input.bin_args.dst = dst;
            break;

        // unary
        case kind_ins_increment:
        case kind_ins_decrement:
        case kind_ins_push:
        case kind_ins_pop:
        case kind_ins_not:
        case kind_ins_neg:
        case kind_ins_jump:
        case kind_ins_call:
        case kind_ins_arithmetic_lshift:
            ins->input.unary_arg = src;
            break;
    }

    return ins;
}

arg_t *imt_make_argument(size_t param1, imt_arg_kind_t kind)
{
    arg_t *arg = NEW(arg_t);
    arg->kind = kind;

    switch (kind)
    {
        case kind_arg_address:
            arg->value.address = (address_t *)param1;
            break;

        case kind_arg_constant:
            arg->value.constant = param1;
            break;

        case kind_arg_register:
            arg->value.reg = param1;
            break;

        case kind_arg_temporary:
            arg->value.temporary = param1;
            break;
    }

    return arg;
}

address_t *imt_make_address(int32_t offset, arg_t *base, 
    arg_t *index, imt_ins_size_t kind_size, imt_address_kind_t kind)
{
    address_t *address = NEW(address_t);
    address->kind = kind;
    
    switch (kind)
    {
        case kind_addr_dereference:
            address->value.deref_address = base;        
            break;

        case kind_addr_offset_derefference:
            address->value.deref_offset.constant = offset;
            address->value.deref_offset.deref_address = base;
            break;

        case kind_addr_complex_derefference:
            address->value.deref_complex.constant = offset;
            address->value.deref_complex.base = base;
            address->value.deref_complex.index = index;
            address->value.deref_complex.size = kind_size;
            break;

        case kind_addr_label:
            break;
    }

    return address;
}

address_t *imt_make_address_label(char *label)
{
    address_t *address = NEW(address_t);
    address->kind = kind_addr_label;
    address->value.label = label;

    return address;
}