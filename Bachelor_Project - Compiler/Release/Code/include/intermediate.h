#ifndef __H_INTERMEDIATE__
#define __H_INTERMEDIATE__

#include "array_list.h"
#include "intermediate_tree.h"
#include "assembler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define DEBUG_INTERMEDIATE(Name) if (intermediate_debug) fprintf(stderr, "Called %s", #Name);

int intermediate_debug;

/* include runtime check, set in main. */
int intermediate_add_check;

/* use more optimal, but less safe algorithm for setting static link. */
int intermediate_simple_static;

/* is used to determine if a compiler generated function is called. */
typedef struct 
{
    int is_used;
    ins_t *function;
} build_in_t;

build_in_t *call_int2string;
build_in_t *call_write;
build_in_t *call_get_static;
build_in_t *call_meminit;
build_in_t *call_allocate_array;
build_in_t *call_allocate_record;
build_in_t *call_array_index;
build_in_t *call_memcopy;
build_in_t *call_memstore;
build_in_t *call_memcopyfrom;
build_in_t *call_sqrt;
build_in_t *call_readstdin;

/* create intermediate tree from ASM tree. */
program_t *im_make_il_program(body_t *asm_tree);
void im_function(void *v_function);
void im_head(void *v_head);
imt_data_kind_t im_type(void *v_type);
void im_var_type(void *v_var_type);
void im_body(void *v_body);
void im_declaration(void *v_declaration);
void im_statement(void *v_statement);
void im_variable(void *v_variable);
void im_exp(void *v_exp);
void im_term(void *v_term);

#endif
