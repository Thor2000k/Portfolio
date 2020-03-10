#include "tree.h"
#include "array_list.h"
#include "memory.h"
#include "lex.h"
#include <string.h>
#include <stdio.h>

extern int lineCount;

function_t *make_function(head_t *head, body_t *body, char *ID)
{
    PRINT_CALL(make_function)
    function_t *t = NEW(function_t);
    t->head = head;
    t->body = body; 

    if (head->ID == NULL)
        fprintf(stderr, "Head ID is NULL\n");        
    if (ID == NULL)
        fprintf(stderr, "ID is NULL\n");
    
    if (strcmp(head->ID, ID) != 0)
    {
        fprintf(stderr, "function name '%s' did not match end '%s' \n", head->ID, ID);
    }

    return t;
}

head_t *make_head(char *ID, array_list *params, type_t *type)
{
    PRINT_CALL(make_head)
    head_t *t = NEW(head_t);
    t->ID = ID;
    t->par_list = params;
    t->return_type = type; 
    return t; 
}

type_t *make_type(void *reference, type_kind_t kind)
{
    PRINT_CALL(make_type)
    type_t *t = NEW(type_t);
    t->kind = kind;
    t->reference = reference;

    return t;
}

var_type_t *make_var_type(char *ID, type_t *type)
{
    PRINT_CALL(make_var_type)
    var_type_t *t = NEW(var_type_t); 
    t->ID = ID;
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

statement_t *make_statement(void *param1, void *param2, void *param3, statement_kind_t kind)
{
    PRINT_CALL(make_statement)
    statement_t *t = NEW(statement_t); 
    t->lineno = lineCount; 
    t->kind = kind; 

    switch(kind)
    {
        case kind_return:
            t->value.expression = param1; 
        break;
        case kind_write:
            t->value.expression = param1; 
        break;
        case kind_allocate_record:
            t->value.variable = param1;
        break;
        case kind_allocate_array:
            t->value.allocate_array.variable = param1;
            t->value.allocate_array.expression = param2; 
        break;
        case kind_assign:
            t->value.assign.variable = param1;
            t->value.assign.expression = param2; 
        break;
        case kind_if:
            t->value.if_statement.expression = param1;
            t->value.if_statement.statement = param2;
        break; 
        case kind_if_else:
            t->value.if_else.expression = param1;
            t->value.if_else.if_statement = param2;
            t->value.if_else.else_statement = param3; 
        break;
        case kind_while:
            t->value.while_statement.expression = param1;
            t->value.while_statement.statement = param2;
        break;
        default: t->value.statement_list = param1; break;
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
    if(kind == kind_term)
    {
        t->kind = kind;
        t->value.term = param1;
        t->lineno = lineCount;
        return t; 
    }

    t->kind = kind;
    t->value.bin_exp.left = param1; 
    t->value.bin_exp.right = param2; 
    t->lineno = lineCount;

    return t; 

}

term_t *make_term(void *param1, void *param2, term_kind_t kind)
{
    PRINT_CALL(make_term)
    int *num;
    term_t *t = NEW(term_t);
    t->kind = kind;        
    t->lineno = lineCount;

    // functions like an if (kind == case1) ... else if (kind == case2) ...  
    switch (kind)
    {
        case kind_method:
            t->value.method.ID = param1;
            t->value.method.actions = param2;
            break;
        case kind_integer:
            num = param1;
            t->value.num = *num;
            break;
        case kind_true: t->value.boolean = T_TRUE; break;
        case kind_false: t->value.boolean = T_FALSE; break;
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
