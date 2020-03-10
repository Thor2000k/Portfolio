#include "tree.h"
#include "array_list.h"
#include "memory.h"
#include "lex.h"
#include "treeweed.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

extern int lineCount;
extern int parsePassed;

function_t *make_function(head_t *head, body_t *body, char *ID)
{
    PRINT_CALL(make_function)
    function_t *t = NEW(function_t);
    t->head = head;
    t->body = body; 
    t->imt_section = NULL;

    if (head->ID == NULL)
        fprintf(stderr, "Head ID is NULL\n");        
    if (ID == NULL)
        fprintf(stderr, "ID is NULL\n");
    
    if (strcmp(head->ID, ID) != 0)
    {
        fprintf(stderr, "function name '%s' in line %d did not match end '%s' in line %d.\n", 
            head->ID, head->lineno, ID, lineCount);

        parsePassed = 0;
    }

    return t;
}

head_t *make_head(char *ID, array_list *params, type_t *type)
{
    PRINT_CALL(make_head)
    head_t *t = NEW(head_t);

    t->lineno = lineCount;
    t->ID = ID;
    t->par_list = params;
    t->type = type; 
    return t; 
}

type_t *make_type(void *reference, type_kind_t kind)
{
    PRINT_CALL(make_type)
    type_t *t = NEW(type_t);
    t->lineno = lineCount;
    t->kind = kind;

    switch (kind)
    {
        case kind_t_identifier:
            t->value.ID = reference;
            break;

        case kind_t_array:
            t->value.array_type = reference;
            break;

        case kind_t_record:
            t->value.record.var_list = reference;
            t->value.record.table = sym_init_table();
    
        default:
            break;
    }

    return t;
}

var_type_t *make_var_type(char *ID, type_t *type)
{
    PRINT_CALL(make_var_type)
    var_type_t *t = NEW(var_type_t); 
    t->ID = ID;
    t->lineno = lineCount;
    t->type = type; 
    return t; 
}

body_t *make_body(array_list *decl_list, array_list *statement_list)
{
    PRINT_CALL(make_body)
    body_t *t = NEW(body_t);
    t->decl_list = decl_list;
    t->stat_list = statement_list;
    return t; 
}

statement_t *make_statement(statement_kind_t kind, int argc, ...)
{
    va_list valist;
    PRINT_CALL(make_statement)
    statement_t *t = NEW(statement_t); 
    t->lineno = lineCount; 
    t->kind = kind; 

    va_start(valist, argc);

    switch(kind)
    {
        case kind_return:
            t->value.expression = va_arg(valist, exp_t *); 
            break;
        case kind_write:
            t->value.expression = va_arg(valist, exp_t *); 
            break;
        case kind_read:
            t->value.variable = va_arg(valist, variable_t *);
            break;
        case kind_allocate_record:
            t->value.variable = va_arg(valist, variable_t *);
            break;
        case kind_allocate_array:
            t->value.allocate_array.variable = va_arg(valist, variable_t *);
            t->value.allocate_array.expression = va_arg(valist, exp_t *); 
            break;
        case kind_assign:
            t->value.assign.variable = va_arg(valist, variable_t *);
            t->value.assign.expression = va_arg(valist, exp_t *); 
            break;
        case kind_if:
            t->value.if_statement.expression = va_arg(valist, exp_t *);
            t->value.if_statement.statement = va_arg(valist, statement_t *);
            break; 
        case kind_if_else:
            t->value.if_else.expression = va_arg(valist, exp_t *);
            t->value.if_else.if_statement = va_arg(valist, statement_t *);
            t->value.if_else.else_statement = va_arg(valist, statement_t *); 
            break;
        case kind_while:
            t->value.while_statement.expression = va_arg(valist, exp_t *);
            t->value.while_statement.statement = va_arg(valist, statement_t *);
            break;
        case kind_statement_list:
            t->value.statement_list = va_arg(valist, array_list *); 
            break;
        case kind_copy:
            t->value.copy.src = va_arg(valist, variable_t *);
            t->value.copy.dst = va_arg(valist, variable_t *);
            break;
        case kind_copy_from:
            t->value.copy_from.src = va_arg(valist, variable_t *);
            t->value.copy_from.src_index = va_arg(valist, exp_t *);
            t->value.copy_from.dst = va_arg(valist, variable_t *);
            t->value.copy_from.dst_index = va_arg(valist, exp_t *);
            t->value.copy_from.count = va_arg(valist, exp_t *);
            break;
        default: 
            break;
    }
    return t; 
}

declaration_t *make_declaration(void *param1, void *param2, declaration_kind_t kind)
{
    PRINT_CALL(make_declaration)
    declaration_t *t = NEW(declaration_t);
    t->lineno = lineCount;
    t->kind = kind; 

    switch(kind)
    {
        case kind_typedef:
            t->value.assign_type.ID = param1;
            t->value.assign_type.type = param2; 
        break;
        case kind_function:
            t->value.function = param1;
        break;
        default: t->value.var_list = param1; break; 
    }
    return t;
}


variable_t *make_variable(void *param1, void *param2, variable_kind_t kind)
{
    PRINT_CALL(make_variable)
    variable_t *t = NEW(variable_t);
    t->type = NULL;
    t->temp_arg = NULL;
    t->lineno = lineCount;
    t->kind = kind; 

    switch(kind)
    {
        case kind_index:
            t->value.get_index.variable = param1;
            t->value.get_index.exp = param2;
            break;
        case kind_field:
            t->value.field.variable = param1;
            t->value.field.ID = param2; 
            break; 
        default: t->value.ID = param1; break; 
    }
    return t; 
    
}

exp_t *make_exp(void *param1, void *param2, exp_kind_t kind)
{
    PRINT_CALL(make_exp)
    exp_t *t = NEW(exp_t);
    
    t->is_reduced = 0;
    t->lineno = lineCount;
    t->kind = kind;
    t->sub_expressions = 0;
    
    if(kind == kind_term)
    {
        t->value.term = param1;
        return t; 
    }

    t->value.bin_exp.left = param1; 
    t->value.bin_exp.right = param2; 

    return t; 

}

term_t *make_term(void *param1, void *param2, term_kind_t kind)
{
    PRINT_CALL(make_term)
    term_t *t = NEW(term_t);
    t->kind = kind;        
    t->lineno = lineCount;
    t->sub_expressions = 0;

    switch (kind)
    {
        case kind_method:
            t->value.method.ID = param1;
            t->value.method.actions = param2;
            break;
        case kind_integer:
            t->value.num = *(int *)param1;
            break;
        case kind_true: t->value.boolean = 1; break;
        case kind_false: t->value.boolean = 0; break;
        case kind_null: break;
        default: t->value.ptr = param1; break;
    }

    return t; 
}

array_list *append_list(array_list *list, void *element)
{
    PRINT_CALL(append_list)
    if( list == NULL )
        list = al_init_list(5, sizeof(void *));
    
    al_add(list, &element);
    return list; 
}
