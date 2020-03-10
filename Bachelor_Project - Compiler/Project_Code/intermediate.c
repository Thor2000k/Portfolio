#include "intermediate.h"
#include "intermediate_tree.h"
#include "symbol.h"
#include "tree.h"
#include "hash_table.h"
#include "typecheck.h"
#include <string.h>

#include "pretty_printer.h"

symbol_table *current_table;
function_section_t *current_funct = NULL;

array_list *text_section;
array_list *offset_table;
hash_table_t *record_queue;
array_list *function_collection;

int current_callee_reg = 0;
hash_table_t *prefix_table;

int register_is_used[16];
arg_t *ZERO;
arg_t *ONE;

int current_static_jumps;
int static_has_been_set;
int offset_table_next = 0;

int temp_index;

int do_swap;
array_list *pop_args;
arg_t *swap_arg;

// Local method declarations
static arg_t *im_allocate_temp();
static entry_t *im_add_ins(ins_t *ins);
static void im_add_assign(arg_t *src, arg_t *dst);
static void im_add_push(arg_t *arg);
static void im_add_pop(arg_t *arg);
static void im_add_cond_jump(arg_t *left, arg_t *right, char *label);
static void im_set_static_link_function(int jumps);
static void im_set_static_link_variable(int jumps);
static void im_declaration_scan(symbol_table *table);
static void im_assign_offsets(symbol_table *table, int start_offset, 
    int direction);
static int im_assign_symbol(symbol_t *symbol, int offset);
static void im_assign_parameters(function_t *function);
static void im_assign_record(void *v_type);
static void im_preamble(array_list *local_symbols, arg_t *static_link, arg_t *offset_table_start);

void im_list(array_list *list, void (*im_f)(void *node))
{
    DEBUG_INTERMEDIATE(im_list)

    if (list == NULL)
    {
        return;
    }

    void *ptr;
    int i = 0;
    
    AL_FOR_EACH(void *, ptr, list, i)
        im_f(ptr);
}

static build_in_t *im_make_build_in(char *name)
{
    build_in_t *func = NEW(build_in_t);
    func->function = MAKE_INS_UNARY(MAKE_LABEL(malloc_string(name)), 
        kind_ins_call, kind_bytes_not_set);

    func->is_used = 0;

    return func;
}

static void im_add_call(build_in_t *func)
{
    func->is_used = 1;
    im_add_ins(func->function);
}

static char *im_make_label(char *prefix, char *name)
{
    int count = ht_get_int(prefix_table, prefix, 1);
    int prefix_length = strlen(prefix);
    int name_length = strlen(name);

    char *buffer = Malloc(prefix_length + name_length + 10);
    sprintf(buffer, "%s%d_%s", prefix, count, name);

    ht_put_int(prefix_table, prefix, count + 1);

    return buffer;
}

void im_add_comment(char *comment)
{
    char *string = malloc_string(comment);
    entry_t *entry = imt_make_entry(string, kind_ent_comment);
    al_add(text_section, &entry);
}

static entry_t *im_add_ins(ins_t *ins)
{
    entry_t *entry = imt_make_entry(ins, kind_ent_instruction);
    al_add(text_section, &entry);
    return entry;
}

static arg_t *im_add_bin_result(arg_t *src, arg_t *dst, imt_ins_kind_t kind, 
    imt_ins_size_t size)
{
    arg_t *result_arg;
    if (src->kind == kind_arg_temporary)
    {
        result_arg = src;
    }
    else
    {
        result_arg = im_allocate_temp();
        im_add_assign(src, result_arg);
    }
    
    im_add_ins(imt_make_instruction(dst, result_arg, NULL, kind, size));

    return result_arg;
}

static void im_add_empty()
{
    entry_t *entry = imt_make_entry("", kind_ent_empty);
    al_add(text_section, &entry);
}

static void im_add_tag(imt_tag_kind_t tag)
{
    entry_t *entry = imt_make_entry(&tag, kind_ent_tag);
    al_add(text_section, &entry);
}

static entry_t *im_add_label(char *label)
{
    if (!intermediate_simple_static)
        static_has_been_set = 0;
    
    entry_t *entry = imt_make_entry(label, kind_ent_label);
    al_add(text_section, &entry);
    return entry;
}

// left = right -> movq right, left
static void im_add_assign(arg_t *src, arg_t *dst)
{
    if (src == NULL || dst == NULL)
        return;

    arg_t *arg;

    if (src->kind == kind_arg_address && dst->kind == kind_arg_address)
    {
        arg = im_allocate_temp();
        im_add_ins(MAKE_INS_BINARY(src, arg, kind_ins_assign, kind_byte8));
        im_add_ins(MAKE_INS_BINARY(arg, dst, kind_ins_assign, kind_byte8));
    }
    else
    {
        im_add_ins(MAKE_INS_BINARY(src, dst, kind_ins_assign, kind_byte8));
    }
}


static void im_add_push(arg_t *arg)
{
    if (arg == NULL)
        return;

    im_add_ins(MAKE_INS_UNARY(arg, 
        kind_ins_push, kind_bytes_not_set));
}

static void im_add_pop(arg_t *arg)
{
    if (arg == NULL)
        return;
        
    im_add_ins(MAKE_INS_UNARY(arg, 
        kind_ins_pop, kind_bytes_not_set));
}

static void im_add_cond_jump(arg_t *left, arg_t *right, char *label)
{
    if (left == NULL || right == NULL)
        return;

    if (left->kind != kind_arg_constant && right->kind == kind_arg_constant)
    {
        arg_t *temp = left;
        left = right;
        right = temp;
    }
    else if (left->kind == kind_arg_constant && 
        right->kind == kind_arg_constant)
    {
        arg_t *temp = im_allocate_temp();
        im_add_assign(right, temp);
        right = temp;
    }
    
    im_add_ins(imt_make_instruction(left, right, label, 
        kind_ins_cond_jump, kind_bytes_not_set));
}

static void im_set_static_link_function(int jumps)
{   
    if (jumps == current_static_jumps && static_has_been_set)
    {
        im_add_assign(asm_arg_table[R9], asm_arg_table[RAX]);
    }
    else if (jumps == 0)
    {
        im_add_assign(asm_arg_table[RBP], asm_arg_table[RAX]);
    }
    else 
    {
        im_add_assign(MAKE_CONSTANT(jumps), asm_arg_table[RDI]);
        im_add_call(call_get_static);
    }
}

static void im_set_static_link_variable(int jumps)
{
    if (jumps == current_static_jumps && static_has_been_set)
    {
        return;
    }
    else
    {
        static_has_been_set = 1;
        current_static_jumps = jumps;
    }

    if (jumps == 0)
    {
        im_add_assign(asm_arg_table[RBP], asm_arg_table[R9]);        
    }
    else
    {
        im_add_assign(MAKE_CONSTANT(jumps), asm_arg_table[RDI]);
        im_add_call(call_get_static);
        im_add_assign(asm_arg_table[RAX], asm_arg_table[R9]);                
    }    
}

static arg_t *im_allocate_temp()
{
    arg_t *temp = imt_make_argument(temp_index, kind_arg_temporary);
    temp_index++;

    return temp;
}

static void im_initialize()
{
    call_int2string = im_make_build_in("int2string");
    call_write = im_make_build_in("write");
    call_get_static = im_make_build_in("get_static");
    call_meminit = im_make_build_in("meminit");
    call_allocate_array = im_make_build_in("allocate_array");
    call_allocate_record = im_make_build_in("allocate_record");
    call_array_index = im_make_build_in("array_index");
    call_memcopy = im_make_build_in("memcopy");
    call_memstore = im_make_build_in("memstore");
    call_memcopyfrom = im_make_build_in("memcopyfrom");
    call_sqrt = im_make_build_in("sqrt");
    call_readstdin = im_make_build_in("readstdin");

    ZERO = MAKE_CONSTANT(0);
    ONE = MAKE_CONSTANT(1);

    prefix_table = ht_init_hash_table(kind_hash_int, kind_ht_key_string);
    record_queue = ht_init_hash_table(kind_hash_int, kind_ht_key_pointer);
    pop_args = AL_INIT_PTR(4);

    for (int i = 0; i < 16; i++)
        register_is_used[i] = 0;
}

program_t *im_make_il_program(body_t *asm_tree)
{
    im_initialize();

    program_t *program = imt_make_program();
    current_table = asm_tree->table;    

    // data_section = program->data_section;
    array_list *fc = AL_INIT_PTR(5);
    data_decl_t *decl = imt_make_data_decl(malloc_string("offset_table"), 
        50, kind_data_pointer);
    offset_table = decl->values;
    function_collection = fc;
    text_section = program->text_section;

    // scan all declarations and set offsets
    im_declaration_scan(current_table);

    // set symbols in record
    im_list(ht_get_keys(record_queue), &im_assign_record);

    // get local variables
    array_list *variable_queue = sym_get_all(current_table, kind_s_var);

    // create stack frame preamble
    im_preamble(variable_queue, ZERO, ZERO);

    im_add_empty();
    im_add_comment("Init memory");
    im_add_call(call_meminit);

    im_add_empty();
    im_add_comment("Main scope code");

    // create IL for main scope code
    im_list(asm_tree->stat_list, &im_statement);

    // change to collection of function_section_t
    function_collection = program->functions;

    // create IL for functions
    im_list(fc, &im_function);

    // add offset table to data declaration
    al_add(program->data_section, &decl);

    al_destroy(variable_queue, NULL);
    return program;
}

static void im_declaration_scan(symbol_table *table)
{
    // create local offsets for table
    im_assign_offsets(table, -16, -1);

    // create queues
    array_list *func_queue = sym_get_all(table, kind_s_function);

    symbol_t *symbol;
    function_t *function;
    char *name;
    int i;

    AL_FOR_EACH(symbol_t *, symbol, func_queue, i)
    {
        function = symbol->value.func;
        name = function->head->ID;
        function->imt_section = imt_make_function(function, 
            im_make_label("f", name), im_make_label("end", name), offset_table_next);

        al_add(function_collection, &function);
        // decide offset for parameters
        im_assign_parameters(function);
        im_declaration_scan(function->body->table);
    }

    al_destroy(func_queue, NULL);
}

static void im_assign_offsets(symbol_table *table, int start_offset, int direction)
{
    array_list *variable_queue = sym_get_all(table, kind_s_var);    
    array_list *pointer_symbols = AL_INIT_PTR(10);
    symbol_t *symbol;
    type_kind_t kind;
    type_t *type;
    
    // decide offset for local variables
    int offset = start_offset;
    int i = 0;
    
    AL_FOR_EACH(symbol_t *, symbol, variable_queue, i)
    {
        kind = symbol->value.type->kind; 
        if (kind == kind_t_array)
        {
            type = symbol->value.type;
            while (kind == kind_t_array)
            {
                type = type->value.array_type;
                kind = type->kind;
            }

            if (kind == kind_t_record)
                ht_put_int(record_queue, type, 0);
            else if (kind == kind_t_symbol)
                ht_put_int(record_queue, type->value.symbol->value.type, 0);

            al_add(pointer_symbols, &symbol);
        }
        else if (kind == kind_t_record)
        {
            ht_put_int(record_queue, symbol->value.type, 0);
            al_add(pointer_symbols, &symbol);
        }
        else if (kind == kind_t_symbol)
        {
            ht_put_int(record_queue, symbol->value.type->value.symbol->value.type, 0);
            al_add(pointer_symbols, &symbol);
        }

        if (!symbol->is_parameter && im_assign_symbol(symbol, offset))
            offset += direction * 8;
    }

    int64_t value = pointer_symbols->size;

    al_add(offset_table, &value);
    offset_table_next += 8;

    // add ofset for pointer variables to offset table
    AL_FOR_EACH(symbol_t *, symbol, pointer_symbols, i)
    {
        if (symbol->longest_jump != -1)
        {
            value = symbol->arg->value.address->value.deref_offset.constant;
            al_add(offset_table, &value);
            offset_table_next += 8;
        }
    }

    al_destroy(variable_queue, NULL);
    al_destroy(pointer_symbols, NULL);
}

static int im_assign_symbol(symbol_t *symbol, int offset)
{
    if (symbol->longest_jump == -1 || symbol->arg != NULL)
        return 0;

    // if used outside of local scope use R9
    asm_registers_t reg = symbol->longest_jump == 0 ? RBP : R9;
        
    symbol->arg = MAKE_DEREF_OFF(asm_arg_table[reg], offset); 

    return 1;
}

static void im_assign_parameters(function_t *function)
{    
    array_list *par_list = function->head->par_list;
    if (par_list == NULL)
        return;
    
    var_type_t *var_type;
    symbol_t *symbol;
    int offset = 24;
    int i;

    AL_FOR_EACH_REVERSE(var_type_t *, var_type, par_list, i)
    {
        sym_get(function->body->table, var_type->ID, &symbol, 0);
        symbol->is_parameter = 1;
        im_assign_symbol(symbol, offset);
        offset += 8;
    }
}

static void im_assign_record(void *v_type)
{
    if (v_type == NULL)
    {
        return;
    }

    type_t *type = v_type;

    type->offset_table_start = offset_table_next;

    symbol_table *table = type->value.record.table;
    int64_t value = table->hash_table->size * 8;
    type->size = value;

    al_add(offset_table, &value);
    offset_table_next += 8;
    
    symbol_t *symbol;
    var_type_t *var_type;
    array_list *var_list = type->value.record.var_list;
    array_list *pointer_list = AL_INIT_PTR(var_list->size);

    int offset = 16;
    int i;
    AL_FOR_EACH(var_type_t *, var_type, var_list, i)
    {
        sym_get(table, var_type->ID, &symbol, 0);
        if (check_is_valid(checks->is_pointer, var_type->type))
            al_add(pointer_list, &symbol);

        symbol->arg = MAKE_CONSTANT(offset);

        offset += 8;
    }
    
    value = pointer_list->size;
    al_add(offset_table, &value);
    offset_table_next += 8;

    AL_FOR_EACH(symbol_t *, symbol, pointer_list, i)
    {
        value = symbol->arg->value.constant;
        al_add(offset_table, &value);
        offset_table_next += 8;
    }

    al_destroy(pointer_list, NULL);
}

static void im_preamble(array_list *local_symbols, arg_t *static_link, 
    arg_t *offset_table_start)
{
    im_add_comment("Preamble");

    // push rbp
    im_add_push(asm_arg_table[RBP]);
    
    // push recieved static link
    im_add_push(static_link);

    // rbp = rsp
    im_add_assign(asm_arg_table[RSP], asm_arg_table[RBP]);

    // push recieved table offset
    im_add_push(offset_table_start);

    // r9 = rbp
    im_add_assign(asm_arg_table[RBP], asm_arg_table[R9]);
    static_has_been_set = 1;
    current_static_jumps = 0;

    int size = 0;
    symbol_t *symbol;

    for (int i = 0; i < local_symbols->size; i++)
    {
        AL_GET_PTR(symbol, local_symbols, i);
        if (!symbol->is_parameter && symbol->longest_jump >= 0)
            size++;
    }

    if (size == 0)
        return;

    // allocate stack space for variables, rsp = rsp - size * 8
    im_add_ins(imt_make_instruction(MAKE_CONSTANT(size * 8), asm_arg_table[RSP],
        NULL, kind_ins_sub, kind_bytes_not_set));

    im_add_empty();
    im_add_comment("Assigning default values");

    // assign default values
    for (int i = 0; i < local_symbols->size; i++)
    {
        AL_GET_PTR(symbol, local_symbols, i);
        if (symbol->is_parameter || symbol->longest_jump == -1)
            continue;

        im_add_assign(ZERO, symbol->arg);
    }
}

void im_function(void *v_function)
{
    DEBUG_INTERMEDIATE(im_function)
    if (v_function == NULL)
    {        
        return;
    }

    function_t *function = v_function;
    current_funct = function->imt_section;

    al_add(function_collection, &current_funct);
    current_table = function->body->table;    
    text_section = current_funct->body;  
    
    im_add_label(current_funct->name);

    array_list *variable_queue = sym_get_all(current_table, kind_s_var);
    im_preamble(variable_queue, asm_arg_table[RAX], 
        MAKE_CONSTANT(current_funct->offset_table_start));

    im_add_empty();
    im_add_comment("Function body");

    im_list(function->body->stat_list, &im_statement);

    im_add_label(current_funct->end_label);

    im_add_assign(asm_arg_table[RBP], asm_arg_table[RSP]);
    im_add_ins(imt_make_instruction(MAKE_CONSTANT(8), asm_arg_table[RSP], NULL,
        kind_ins_add, kind_bytes_not_set));

    im_add_pop(asm_arg_table[RBP]);
}

imt_data_kind_t im_type(void *v_type)
{
    DEBUG_INTERMEDIATE(im_type)
    if (v_type == NULL)
    {        
        return kind_data_pointer;
    }

    type_t *type = v_type;
    
    // Currently all pointer / 8 byte size 
	switch(type->kind)
    {
		case kind_t_integer:
            return kind_data_pointer;
            break;

		case kind_t_boolean:
            return kind_data_pointer;
            break;
            
		case kind_t_array:            
		case kind_t_record:            
		case kind_t_identifier:            
        case kind_t_undeclared:
        case kind_t_symbol:
            return kind_data_pointer;
            break;

		default:
            break;            	
    }

    // Anything can fit 8bytes.
    return kind_data_pointer;
}

void im_statement(void *v_statement)
{
    DEBUG_INTERMEDIATE(im_statement)

    statement_t *statement = v_statement;
    
    temp_index = 0;
    im_add_tag(kind_tag_statement_start);

    exp_t *exp;
    variable_t *variable;
    arg_t *arg;
    char *label_end;
    char *label_cond;

    switch (statement->kind)
    {
        case kind_return:
            im_add_empty();   

            exp = statement->value.expression;
            im_exp(exp);

            im_add_assign(exp->temp_arg, asm_arg_table[RAX]);
            im_add_ins(MAKE_INS_UNARY(MAKE_LABEL(current_funct->end_label), 
                kind_ins_jump, kind_bytes_not_set));
            break;

        case kind_write:
            im_add_empty();
            exp = statement->value.expression;
            im_exp(exp);

            if (exp->sub_expressions == 0)
                im_add_empty();

            // rdi = exp
            im_add_assign(exp->temp_arg, asm_arg_table[RDI]);

            // call int2string
            im_add_call(call_int2string);

            // rdi = rax
            im_add_assign(asm_arg_table[RAX], asm_arg_table[RDI]);

            // call write
            im_add_call(call_write);
            break;

        case kind_read:
            variable = statement->value.variable;
            im_variable(variable);
            im_add_call(call_readstdin);            
            im_add_assign(asm_arg_table[RAX], variable->temp_arg);
            break;

        case kind_allocate_record:
            im_add_empty();
            variable = statement->value.variable;
            im_variable(variable);

            // Deallocate variable first
            im_add_assign(ZERO, variable->temp_arg);
            im_add_assign(MAKE_CONSTANT(variable->type->size), asm_arg_table[RDI]);
            im_add_assign(MAKE_CONSTANT(variable->type->offset_table_start), asm_arg_table[RSI]);            
            im_add_call(call_allocate_record);

            im_variable(variable);
            im_add_assign(asm_arg_table[RAX], variable->temp_arg);

            break;

        case kind_allocate_array:
            im_add_empty();
            variable = statement->value.allocate_array.variable;

            exp = statement->value.allocate_array.expression;
            im_exp(exp);

            im_variable(variable);

            // Deallocate variable first
            im_add_assign(ZERO, variable->temp_arg);
            
            im_add_assign(exp->temp_arg, asm_arg_table[RDI]);
            im_add_assign(MAKE_CONSTANT(8), asm_arg_table[RSI]); 
            im_add_assign(MAKE_CONSTANT(check_is_valid(checks->is_pointer, 
                variable->type->value.array_type) + 1), asm_arg_table[RDX]);
            
            im_add_call(call_allocate_array);

            im_variable(variable);
            im_add_assign(asm_arg_table[RAX], variable->temp_arg);

            break;

        case kind_assign:
            exp = statement->value.assign.expression;
            variable = statement->value.assign.variable;
            im_exp(exp);
            im_variable(variable);
            im_add_assign(exp->temp_arg, variable->temp_arg);

            break;

        case kind_if:
            im_add_empty();
            exp = statement->value.if_statement.expression;  
            label_end = im_make_label("if", "end");     

            im_exp(exp);
            im_add_cond_jump(ZERO, exp->temp_arg, label_end);

            im_add_empty();
            im_add_comment("If statement");
            im_statement(statement->value.if_statement.statement);
            im_add_label(label_end);

            break;
            
        case kind_if_else:
            im_add_empty();
            exp = statement->value.if_else.expression;
            label_cond = im_make_label("else", "start");
            label_end = im_make_label("if_else", "end");

            im_exp(exp);
            im_add_cond_jump(ZERO, exp->temp_arg, label_cond);

            im_add_empty();
            im_add_comment("If statement");

            im_statement(statement->value.if_else.if_statement);
            im_add_ins(MAKE_INS_UNARY(MAKE_LABEL(label_end), kind_ins_jump, 
                kind_bytes_not_set));

            im_add_empty();
            im_add_label(label_cond);            
            im_statement(statement->value.if_else.else_statement);
            im_add_label(label_end);

            break;

        case kind_while:
            im_add_empty();
            exp = statement->value.while_statement.expression;
            label_cond = im_make_label("while", "loop");
            label_end = im_make_label("while", "end");

            im_add_label(label_cond);
            im_add_comment("while start");
            im_exp(exp);
            im_add_cond_jump(ZERO, exp->temp_arg, label_end);

            im_add_comment("while body");
            im_statement(statement->value.while_statement.statement);
            im_add_ins(MAKE_INS_UNARY(MAKE_LABEL(label_cond), kind_ins_jump, 
                kind_bytes_not_set));

            im_add_label(label_end);

            break;

        case kind_statement_list:
            im_list(statement->value.statement_list, &im_statement);

            break;

        case kind_copy:
            variable = statement->value.copy.src;

            im_add_comment("Start copy");
            im_variable(variable);
            arg = im_allocate_temp();
            im_add_assign(variable->temp_arg, arg);
            if (intermediate_add_check)
                im_add_cond_jump(ZERO, arg, malloc_string("address_null_err"));

            im_add_empty();
            im_add_comment("Check valid to copy");
            variable = statement->value.copy.dst;
            im_variable(variable);

            im_add_assign(variable->temp_arg, asm_arg_table[RDI]);
            im_add_assign(MAKE_DEREF_OFF(arg, 8), asm_arg_table[RSI]);
            im_add_ins(MAKE_INS_UNARY(asm_arg_table[RSI], kind_ins_decrement, 
                kind_bytes_not_set));
                
            if (intermediate_add_check)
            {
                im_add_call(call_array_index);
            }

            im_add_empty();
            im_add_comment("Perform copy");
            // arg 1, dst
            im_add_ins(imt_make_instruction(MAKE_CONSTANT(16), asm_arg_table[RDI],
                NULL, kind_ins_add, kind_bytes_not_set));

            // arg 3, count
            im_add_assign(asm_arg_table[RSI], asm_arg_table[RDX]);
            im_add_ins(MAKE_INS_UNARY(asm_arg_table[RDX], kind_ins_increment, 
                kind_bytes_not_set));
            im_add_ins(imt_make_instruction(MAKE_CONSTANT(8),asm_arg_table[RDX], 
                NULL, kind_ins_mult, kind_bytes_not_set));

            // arg 2, src
            im_add_assign(arg, asm_arg_table[RSI]);
            im_add_ins(imt_make_instruction(MAKE_CONSTANT(16), asm_arg_table[RSI], 
                NULL, kind_ins_add, kind_bytes_not_set));
            
            im_add_call(call_memcopy);
            im_add_comment("Copy done");
            
            break;

        case kind_copy_from:

            im_add_comment("Start copy");
            // Get variables
            variable = statement->value.copy_from.src;
            im_variable(variable);
            im_add_assign(variable->temp_arg, asm_arg_table[RSI]);
            
            exp = statement->value.copy_from.src_index;
            im_exp(exp);
            im_add_assign(exp->temp_arg, asm_arg_table[R8]);

            variable = statement->value.copy_from.dst;
            im_variable(variable);
            im_add_assign(variable->temp_arg, asm_arg_table[RDI]);

            exp = statement->value.copy_from.dst_index;
            im_exp(exp);
            im_add_assign(exp->temp_arg, asm_arg_table[RCX]);
            
            exp = statement->value.copy_from.count;
            im_exp(exp);
            im_add_assign(exp->temp_arg, asm_arg_table[RDX]);

            im_add_call(call_memcopyfrom);
            im_add_comment("Copy done");

            break;

        default:
            break;
    }
}

void im_variable(void *v_variable)
{
    DEBUG_INTERMEDIATE(im_variable)
    if (v_variable == NULL)
    {        
        return;
    }

    variable_t *variable = v_variable;

    symbol_t *symbol;
    type_t *type = NULL;
    variable_t *child_variable;
    int jumps;
    arg_t *arg, *arg2;
    exp_t *exp;

    switch (variable->kind)
    {
        case kind_variable:
            jumps = sym_get(current_table, variable->value.ID, &symbol, 0);

            // if from van be in another scope
            if (symbol->longest_jump > 0)
            {
                im_set_static_link_variable(jumps);
            }
            type = symbol->value.type;
            if (type->kind == kind_t_symbol)
            {
                type = type->value.symbol->value.type;
            }

            variable->type->size = type->size;
            variable->type->offset_table_start = type->offset_table_start;
            variable->temp_arg = symbol->arg;

            break;
        case kind_index:            
            child_variable = variable->value.get_index.variable;
            im_variable(child_variable);

            exp = variable->value.get_index.exp;
            im_exp(exp);

            arg = im_allocate_temp();
            arg2 = im_allocate_temp();

            im_add_assign(child_variable->temp_arg, arg);
            im_add_assign(exp->temp_arg, arg2);
            
            if (intermediate_add_check)
            {
                im_add_assign(arg, asm_arg_table[RDI]);
                im_add_assign(arg2, asm_arg_table[RSI]);
                im_add_call(call_array_index);
            }

            type = variable->type;
            if (type->kind == kind_t_symbol)
            {
                type = type->value.symbol->value.type;
            }

            variable->type->size = type->size;
            variable->type->offset_table_start = type->offset_table_start;
            
            variable->temp_arg = imt_make_argument(
                (size_t)imt_make_address(16, arg, arg2, kind_byte8, 
                kind_addr_complex_derefference), kind_arg_address);

            break;

        case kind_field:
            child_variable = variable->value.field.variable;
            im_variable(child_variable);

            // get symbol of field
            if (child_variable->type->kind == kind_t_symbol)
                type = child_variable->type->value.symbol->value.type;
            else if (child_variable->type->kind == kind_t_record)
                type = child_variable->type;

            sym_get(type->value.record.table, 
                variable->value.field.ID, &symbol, 0);

            // Check for null
            if (intermediate_add_check)
            {
                im_add_cond_jump(child_variable->temp_arg, ZERO, 
                    malloc_string("address_null_err"));
            }

            // get temporary for address
            arg = im_allocate_temp();

            im_add_assign(child_variable->temp_arg, arg);
            
            variable->type->size = type->size;
            variable->type->offset_table_start = type->offset_table_start;
            variable->temp_arg = MAKE_DEREF_OFF(arg, symbol->arg->value.constant);

            break;
    
        default:
            break;
    }
}

void im_exp(void *v_exp)
{
    DEBUG_INTERMEDIATE(im_exp)
    if (v_exp == NULL)
    {        
        return;
    }

    exp_t *exp = v_exp;
    
    if (exp->kind == kind_term)
    {        
        im_term(exp->value.term);
        exp->temp_arg = exp->value.term->temp_arg;
        return;
    }

    exp_t *left, *right;
    char *label_cond;
    char *label_end;
    arg_t *result_arg;

    left = exp->value.bin_exp.left;
    right = exp->value.bin_exp.right;

    // Short curcuit
    switch (exp->kind)
    {
        case kind_and:
            label_cond = im_make_label("and", "false");
            label_end = im_make_label("and", "end");

            im_exp(left);
            im_add_cond_jump(ZERO, left->temp_arg, label_cond);
            
            im_exp(right);
            im_add_cond_jump(ZERO, right->temp_arg, label_cond);
            
            result_arg = im_allocate_temp();

            im_add_assign(ONE, result_arg);
            im_add_ins(MAKE_INS_UNARY(MAKE_LABEL(label_end), 
                kind_ins_jump, kind_bytes_not_set));
            
            im_add_label(label_cond);
            im_add_assign(ZERO, result_arg);

            im_add_label(label_end);

            exp->temp_arg = result_arg;

            return;
            break;

        case kind_or:
            label_cond = im_make_label("or", "true");
            label_end = im_make_label("or", "end");

            im_exp(left);
            im_add_cond_jump(ONE, left->temp_arg, label_cond);
            
            im_exp(right);
            im_add_cond_jump(ONE, right->temp_arg, label_cond);

            result_arg = im_allocate_temp();

            im_add_assign(ZERO, result_arg);
            im_add_ins(MAKE_INS_UNARY(MAKE_LABEL(label_end), 
                kind_ins_jump, kind_bytes_not_set));
            
            im_add_label(label_cond);
            im_add_assign(ONE, result_arg);

            im_add_label(label_end);

            exp->temp_arg = result_arg;

            return;
            break;

        default:
            break;
    }

    // Other binary operators
    im_exp(left);
    im_exp(right);

    im_add_empty();
    arg_t *constant_arg;
    exp_t *temp_swap;

    // array concatenation 
    if(exp->kind == kind_plus && exp->type->kind == kind_t_array){
        // null check for arrays

        // address of arrays 
        arg_t *ltmp = im_allocate_temp();
        arg_t *rtmp = im_allocate_temp();
        im_add_assign(left->temp_arg, ltmp);
        im_add_assign(right->temp_arg, rtmp);

        if (intermediate_add_check)
        {
            im_add_cond_jump(ZERO, ltmp, malloc_string("address_null_err"));
            im_add_cond_jump(ZERO, rtmp, malloc_string("address_null_err"));
        }

        // size for new array returned in %rdi for allocate_array
        im_add_assign(MAKE_DEREF_OFF(ltmp, 8), asm_arg_table[RDI]);
        im_add_ins(imt_make_instruction(MAKE_DEREF_OFF(rtmp, 8), asm_arg_table[RDI], 
            NULL, kind_ins_add, kind_bytes_not_set));
        
        // size of elements in RSI for allocate_array
        im_add_assign(MAKE_CONSTANT(8), asm_arg_table[RSI]);

        // type of array
        im_add_assign(MAKE_CONSTANT(check_is_valid(checks->is_pointer, 
            left->type->value.array_type)), asm_arg_table[RDX]);
        
        // address of new array in %rax
        im_add_call(call_allocate_array);

        // save address
        im_add_push(asm_arg_table[RAX]);

        // re-allocate temps in case of garbage collection
        im_exp(left);
        im_exp(right);
        ltmp = im_allocate_temp();
        rtmp = im_allocate_temp();
        im_add_assign(left->temp_arg, ltmp);
        im_add_assign(right->temp_arg, rtmp);

        // src start of array
        im_add_assign(MAKE_DEREF(asm_arg_table[RSP]), asm_arg_table[RDI]);
        im_add_ins(imt_make_instruction(MAKE_CONSTANT(16), asm_arg_table[RDI], NULL, 
            kind_ins_add, kind_bytes_not_set));
        
        // should add 16 to adress 
        im_add_assign(ltmp, asm_arg_table[RSI]);
        im_add_ins(imt_make_instruction(MAKE_CONSTANT(16), asm_arg_table[RSI], NULL, 
            kind_ins_add, kind_bytes_not_set));
        
        // start of data in left array
        
        //nr of iterations 
        im_add_assign(MAKE_DEREF_OFF(ltmp, 8), asm_arg_table[RDX]);
        im_add_ins(imt_make_instruction(MAKE_CONSTANT(8), asm_arg_table[RDX], 
            NULL, kind_ins_mult, kind_bytes_not_set));

        im_add_call(call_memcopy); 
 
        // should add 16 to adress 
        im_add_assign(rtmp, asm_arg_table[RSI]);
        im_add_ins(imt_make_instruction(MAKE_CONSTANT(16), asm_arg_table[RSI], NULL, 
            kind_ins_add, kind_bytes_not_set));
        
        // nr of iterations 
        im_add_assign(MAKE_DEREF_OFF(rtmp, 8), asm_arg_table[RDX]);
        im_add_ins(imt_make_instruction(MAKE_CONSTANT(8), asm_arg_table[RDX], 
            NULL, kind_ins_mult, kind_bytes_not_set));

        im_add_call(call_memcopy); 

        im_add_pop(ltmp);
        exp->temp_arg = ltmp;
        return; 
    }
    
    // Left is constant in compare
    switch (exp->kind)
    {
        case kind_equal:
        case kind_not_equal:
            if (left->temp_arg != NULL && left->temp_arg->kind == kind_arg_constant)
            {
                temp_swap = left;
                left = right;
                right = temp_swap;
            }
            break;
        
        case kind_greater:
        case kind_less:
        case kind_greater_equal:
        case kind_less_equal:
            if (left->temp_arg != NULL && left->temp_arg->kind == kind_arg_constant)
            {
                constant_arg = left->temp_arg;
                left->temp_arg = im_allocate_temp();
                im_add_assign(constant_arg, left->temp_arg);
            }
            break;
        
        default:
            break;
    }

    if (exp->kind == kind_divide || exp->kind == kind_modulo)
    {
        // make sure divisor is a register
        if (right->temp_arg != NULL && 
            right->temp_arg->kind != kind_arg_temporary)
        {
            arg_t *temp_arg = im_allocate_temp();

            im_add_assign(right->temp_arg, temp_arg);        
            right->temp_arg = temp_arg;

        }

        // check for zero in divisor
        if (intermediate_add_check)
        {
            im_add_cond_jump(ZERO, right->temp_arg, 
                malloc_string("divide_by_zero_err"));
        }
    }

    // manage how to store temporary result
    result_arg = im_add_bin_result(left->temp_arg, right->temp_arg, 
        (imt_ins_kind_t)exp->kind, kind_bytes_not_set);
    
    exp->temp_arg = result_arg;
}

void im_term(void *v_term)
{
    DEBUG_INTERMEDIATE(im_term)
    if (v_term == NULL)
    {        
        return;
    }

    term_t *term = v_term;

    term_t *child_term;
    exp_t *exp;
    variable_t *variable;
    arg_t *arg;
    array_list *exp_list;
    function_t *function;
    symbol_t *symbol;
    int param_count = 0;
    int jumps;

	switch(term->kind)
	{
		case kind_false:
            term->temp_arg = ZERO;        
			break;

		case kind_true:
            term->temp_arg = ONE;
			break;

		case kind_integer:
            term->temp_arg = MAKE_CONSTANT(term->value.num);
			break;

		case kind_absolute:
            exp = term->value.ptr;
            im_exp(exp);
            
            if (exp->type->kind == kind_t_array)
            {
                if (intermediate_add_check)
                {
                    im_add_cond_jump(ZERO, exp->temp_arg, 
                        malloc_string("address_null_err"));
                }
                    
                arg = im_allocate_temp();
                im_add_assign(exp->temp_arg, arg);
                term->temp_arg = MAKE_DEREF_OFF(arg, 8);
            }
            else
            {
                arg = im_allocate_temp();
                im_add_assign(exp->temp_arg, arg);
                im_add_assign(arg, asm_arg_table[RAX]);
                im_add_ins(MAKE_INS_UNARY(arg, kind_ins_neg, 
                    kind_bytes_not_set));
                
                im_add_ins(MAKE_INS_BINARY(asm_arg_table[RAX], arg, 
                    kind_ins_cond_assign, kind_bytes_not_set));

                term->temp_arg = arg;
            }
			break;

		case kind_negate:
            child_term = term->value.ptr;
            im_term(child_term);

            term->temp_arg = im_add_bin_result(ZERO, child_term->temp_arg, 
                kind_ins_equal, kind_bytes_not_set);
			break;

		case kind_null:
            term->temp_arg = ZERO;
			break;

		case kind_variable:
            variable = term->value.ptr;
			im_variable(variable);
            
            term->temp_arg = variable->temp_arg;
			break;

		case kind_parenthess:
            exp = term->value.ptr;
            im_exp(exp);
            term->temp_arg = exp->temp_arg; 
            break;

		case kind_method:
            exp_list = term->value.method.actions;
            jumps = sym_get(current_table, term->value.method.ID, &symbol, 0);
            function = symbol->value.func;

            im_add_empty();
            im_add_comment("Function call start");
            
            // preserve static link
            im_add_push(asm_arg_table[R9]);

            // make sure any temporary results are preserved
            im_add_tag(kind_tag_func_start);
            
            if (exp_list != NULL)
            {
                for (int i = 0; i < exp_list->size; i++)
                {
                    AL_GET_PTR(exp, exp_list, i);
                    im_exp(exp);
                    im_add_push(exp->temp_arg);
                    param_count += 8;
                }
            }

            // set static link
            im_set_static_link_function(jumps);

            // actually call the function
            im_add_ins(MAKE_INS_UNARY(MAKE_LABEL(function->imt_section->name), 
                kind_ins_call, kind_bytes_not_set));

            // remove parameters from stack
            if (param_count > 0)
            {
                im_add_ins(imt_make_instruction(MAKE_CONSTANT(param_count), 
                    asm_arg_table[RSP], NULL, kind_ins_add, kind_bytes_not_set));
            }
            
            // restore temporary results
            im_add_tag(kind_tag_func_end);

            // restore static link
            im_add_pop(asm_arg_table[R9]);

            // add return value to a temporary result
            arg = im_allocate_temp();
            im_add_assign(asm_arg_table[RAX], arg);
            im_add_comment("Function call end");
            
            term->temp_arg = arg;
			break;

        case kind_sqrt:
            exp = term->value.ptr;
            im_exp(exp);

            im_add_assign(exp->temp_arg, asm_arg_table[RDI]);
            im_add_call(call_sqrt);
            
            arg = im_allocate_temp();
            im_add_assign(asm_arg_table[RAX], arg);

            term->temp_arg = arg;
            break;
	}	      
}
