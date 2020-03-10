#ifndef P_PRINT
#define P_PRINT
#include "tree.h" //includes tree struct in header
#include <stdio.h>
//all structs can be nodes in the tree, the body struct being root)

#define DEBUG_PRETTY 0
#define PRINT_DEBUG(S) if (DEBUG_PRETTY) fprintf(stderr, "Called method %s\n", #S); 
 
#define DEC_INDENT indent_size -= indent_increment
#define INC_INDENT indent_size += indent_increment
#define PREV_SCOPE(TASK) DEC_INDENT; TASK; INC_INDENT;
#define NEXT_SCOPE(TASK) INC_INDENT; TASK; DEC_INDENT; 

void print_indent();
void print_list(array_list *list, char *delim, void (*print_f)(void *node));

void print_function(void *v_function);
void print_head(void *v_head);
void print_type(void *v_type);
void print_var_type(void *v_var_type);
void print_body(void *v_body);
void print_declaration(void *v_declaration);
void print_statement(void *v_statement);
void print_variable(void *v_variable);
void print_exp(void *v_exp);
void print_term(void *v_term);
#endif
