#ifndef __H_TREEWEED__
#define __H_TREEWEED__

#include "tree.h"

// perform an arithmetic operation between L and R and assign the result to T
#define ARI_OP(T, L, R, OP) T->value.num = L->value.num OP R->value.num

// convert a boolean value to an enum
#define CON_ENUM(B) ((B) ? kind_true : kind_false)

// compute boolean operation between L and R and save result in T 
#define BOOL_OP(T, L, R, OP) \
    T->value.boolean = L->value.boolean OP R->value.boolean; \
    T->kind = CON_ENUM(T->value.boolean)

/* checks if an expression is constant. */
int is_constant(exp_t *exp);

/* makes sure terms are of the same type. */
void cast_terms(term_t *left, term_t *right);

/* returns 1 if all calls of return_check returns 1. */
int weed_list(array_list *list, 
    int (*return_check)(void *node));

/* main function to start the weeding process. */
int weed_start(body_t *asm_tree);
int weed_function(void *v_function);
void weed_head(void *v_head);
void weed_type(void *v_type);
void weed_var_type(void *v_var_type);
int weed_body(void *v_body);
int weed_declaration(void *v_declaration);

/* returns 1 if any calls of return_check returns 1. */
int weed_valid_return_list(array_list *list, 
    int (*return_check)(void *node));
int weed_statement(void *v_statement);
void weed_variable(void *v_variable);
void weed_exp(exp_t *exp);
void weed_term(term_t *term);

#endif