#include "treeweed.h"
#include "tree.h"
#include "misc.h"
#include "typecheck.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int is_constant(exp_t *exp)
{
    if (exp->kind != kind_term)
    {
        return 0;
    }

    term_t *term = exp->value.term;

    switch (term->kind)
    {
        case kind_true:
        case kind_false:
        case kind_integer: 
            return 1;
            break;
        default:
            break;
    }

    return 0;
}

// cast so that terms are same type if needed
void cast_terms(term_t *left, term_t *right)
{
    if (left->kind == right->kind)
    {
        return;
    }
}

// returns 1 on empty list and if all calls of check_return was 1
int weed_list(array_list *list, int (*return_check)(void *node))
{
    if (list == NULL)
    {
        return 1;
    }

    int size = list->size;
    void *ptr;

    int result = 1;
    for (int i = 0; i < size; i++)
    {
        AL_GET_PTR(ptr, list, i);
		result = result & return_check(ptr);
    }
	return result;	
}

// returns 1 if any call of check_return was 1
int weed_valid_return_list(array_list *list, int (*return_check)(void *node))
{
    if (list == NULL)
    {
        fprintf(stderr, "NULL list");
        return 0;
    }

    int size = list->size;
    void *ptr;
    int result = 0;
    for (int i = 0; i < size; i++)
    {
        AL_GET_PTR(ptr, list, i);		
        result = result | return_check(ptr);
    }

	return result;	
}

int weed_start(body_t *asm_tree)
{
    // weed all functions in main scope
    int result = weed_list(asm_tree->decl_list, &weed_declaration);

    // weed all statements in main scope
    weed_list(asm_tree->stat_list, &weed_statement);
    
    return result;
}

int weed_function(void *v_function)
{
    function_t *funcion = v_function;

    // weed all nested functions
    int result = weed_list(funcion->body->decl_list,
     &weed_declaration);
     
    // search for return statement in functions scope and weed expressions 
    int has_return = weed_valid_return_list(funcion->body->stat_list
    , &weed_statement);

    // if no guaranteed return statement was found 
    if (!has_return)
    {
        fprintf(stderr, "main:%d  function '%s' doesn't return on all paths.\n", 
            funcion->head->lineno, funcion->head->ID);

        return 0;
    }

    return result && has_return;
}

void weed_head(void *v_head);
void weed_type(void *v_type);
void weed_var_type(void *v_var_type);

int weed_declaration(void *v_declaration)
{
    declaration_t *declaration = v_declaration;

    int result;
    switch (declaration->kind)
    {
        case kind_function:
            result = weed_function(declaration->value.function);

            return result;
            break;
    
        default:
            return 1;
            break;
    }
}

int weed_statement(void *v_statement)
{
    statement_t *statement = v_statement;
    if (v_statement == NULL)
    {        
        fprintf(stderr, "NULL statement\n");
        return 0;
    }

    int i;
    int j;

    switch (statement->kind)
    {
        case kind_return:
            weed_exp(statement->value.expression);
            return 1;
            break;
        
        case kind_write:            
            weed_exp(statement->value.expression);
            return 0;
            break;

        case kind_allocate_array:
            weed_variable(statement->value.allocate_array.variable);
            weed_exp(statement->value.allocate_array.expression);
            return 0;
            break;

        case kind_allocate_record:
            weed_variable(statement->value.variable);
            return 0;
            break;

        case kind_assign:
            weed_variable(statement->value.assign.variable);
            weed_exp(statement->value.assign.expression);
            return 0;
            break;

        case kind_if:            
            weed_exp(statement->value.expression);
            return 0;
            break;

        case kind_if_else:
            weed_exp(statement->value.if_else.expression);
            i = weed_statement(statement->value.if_else.if_statement);
            j = weed_statement(statement->value.if_else.else_statement);

            return i && j;
	        break;

        case kind_while:
            weed_exp(statement->value.expression);
            return 0;
            break;

        case kind_statement_list:  
            i = weed_valid_return_list(statement->value.statement_list, &weed_statement);
           
            return i;
            break;

        default:
			return 0;
            break;
    }
}

void weed_variable(void *v_variable)
{
    variable_t *variable = v_variable;

    switch (variable->kind)
    {
        case kind_index:
            weed_exp(variable->value.get_index.exp);
            break;
        default:
            break;
    }
}

// == Calculate constants ==
void weed_exp(exp_t *exp)
{
    if (exp->is_reduced)
    {
        return;
    }

    if (exp->kind == kind_term)
    {
        weed_term(exp->value.term);
        exp->sub_expressions = exp->value.term->sub_expressions;

        return;
    }

    exp_t *left, *right;

    left = exp->value.bin_exp.left;
    right = exp->value.bin_exp.right;

    weed_exp(left);
    weed_exp(right);

    exp->sub_expressions = left->sub_expressions + right->sub_expressions + 1;

    if (!is_constant(left) || !is_constant(right))
    {
        exp->is_reduced = 1;
        return;
    }
    
    term_t *left_term, *right_term;

    left_term = left->value.term;
    right_term = right->value.term;

    cast_terms(left_term, right_term);

    term_t *term = NEW(term_t);
    term->kind = left_term->kind;
    term->lineno = exp->lineno;
 
    switch (exp->kind)
    {
        case kind_plus: 
            ARI_OP(term, left_term, right_term, +);
            break;

        case kind_minus:
            ARI_OP(term, left_term, right_term, -);
            break;

        case kind_multiply:  
            ARI_OP(term, left_term, right_term, *);
            break;
                      
        case kind_divide:  
            if (right_term->value.num == 0)
            {
                fprintf(stderr, "ERROR: divide by 0, in line %d.\n", exp->lineno);
                return;
            } 
            else
            {                    
                ARI_OP(term, left_term, right_term, /);
            }
            break;            
            
        case kind_greater:     
            BOOL_OP(term, left_term, right_term, >);
            break;

        case kind_less:            
            BOOL_OP(term, left_term, right_term, <);
            break;

        case kind_greater_equal:                     
            BOOL_OP(term, left_term, right_term, >=);
            break;
               
        case kind_less_equal:                  
            BOOL_OP(term, left_term, right_term, <=);
            break;
                  
        case kind_equal:                        
            BOOL_OP(term, left_term, right_term, ==);
            break;
            
        case kind_not_equal:             
            BOOL_OP(term, left_term, right_term, !=);
            break;
            
        case kind_and:     
            BOOL_OP(term, left_term, right_term, &&); 
            break;      

        case kind_or:    
            BOOL_OP(term, left_term, right_term, ||);
            break;

        case kind_modulo:
            ARI_OP(term, left_term, right_term, %);
            break;

        default:
            break;
    }   
    
    exp->kind = kind_term;
    exp->value.term = term;
    exp->is_reduced = 1;
    exp->sub_expressions = 0;

    free(left_term);
    free(left);
    free(right_term);
    free(right);
}

void weed_term(term_t *term)
{
    exp_t *exp = NULL;
    term_t *negate_term = NULL;
    term_t *exp_term = NULL;
    int remove_child = 0;
    switch (term->kind)
    {
        case kind_negate:
            negate_term = term->value.ptr;
            weed_term(negate_term);
            term->sub_expressions = negate_term->sub_expressions;

            switch (negate_term->kind)
            {
                case kind_true:
                case kind_false:
                case kind_integer:
                    break;
                default:
                    return;
                    break;
            }

            break;
        case kind_parenthess:
        case kind_absolute:
            exp = term->value.ptr;
            weed_exp(exp);
            term->sub_expressions = exp->sub_expressions;

            if (!is_constant(exp))
            {
                return;
            }
            break;

        default:
            term->sub_expressions = 0;
            return;
            break;
    }
    
    if (exp != NULL)
        exp_term = exp->value.term;

    switch (term->kind)
    {
        case kind_parenthess:
            term->kind = exp_term->kind;
            term->value = exp_term->value;
            remove_child = 1;
            break;

        case kind_negate:                    
            remove_child = check_is_valid(checks->is_boolean, negate_term->type);
            if (remove_child)
            {                
                term->kind = negate_term->kind == kind_true ? kind_false : kind_true; 
            }
            break;

        case kind_absolute:
            remove_child = check_is_valid(checks->is_calc, exp_term->type);
            if (remove_child && exp_term->kind == kind_integer)
            {
                term->kind = kind_integer;
                term->value.num = abs(exp_term->value.num); 
            }
            break;

        default:
            return;
            break;
    }

    if (remove_child)
    {
        term->sub_expressions = 0;
        if (exp_term != NULL) free(exp_term);
        if (exp != NULL) free(exp);
        if (negate_term != NULL) free(negate_term);
    }
}
