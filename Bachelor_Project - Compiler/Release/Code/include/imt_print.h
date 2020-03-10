#ifndef __H_IMT_PRINT__
#define __H_IMT_PRINT__

#include "intermediate.h"
#include "intermediate_tree.h"
#include <stdio.h>

void imt_print_program(program_t *program);
void imt_print_data_section(data_decl_t *data);
void imt_print_text_section(entry_t *entry);
void imt_print_instruction(ins_t *ins);
void imt_print_argument(arg_t *arg);
void imt_print_address(address_t *addr);
void imt_print_function_section(function_section_t *func);

#endif
