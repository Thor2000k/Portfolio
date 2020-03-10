#ifndef __H_PEEP_HOLE__
#define __H_PEEP_HOLE__

#include "intermediate_tree.h"
#include "hash_table.h"

/* creates a new list from the entry list asm_code with the optimized code. */
array_list *destroy_redundant_code(array_list *asm_code);

/* perform peephole optimization on the intermediate tree. */
void peep_hole_optimizer(program_t *p);

#endif
