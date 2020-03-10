#include "imt_print.h"
#include <stdint.h> 
#include <inttypes.h>

const char* register_names[] = {"RAX","RCX","RDX","RBX","RSI","RDI","RSP",
                        "RBP","R8","R9","R10","R11","R12","R13","R14","R15"}; 


void imt_print_program(program_t *program){
    array_list *data_list = program->data_section; 
    array_list *entry_list = program->text_section; 
    array_list *functions_list = program->functions; 

    data_decl_t *data_elem = NULL; 
    entry_t *entry_elem = NULL;
    function_section_t *func_elem = NULL;
    
    int i;
    AL_FOR_EACH(data_decl_t *, data_elem, data_list, i)
    {
        imt_print_data_section(data_elem);
    }

    AL_FOR_EACH(entry_t *, entry_elem, entry_list, i)
    {
        imt_print_text_section(entry_elem);
    }

    AL_FOR_EACH(function_section_t *, func_elem, functions_list, i)
    {
        imt_print_function_section(func_elem);
    }
}

void imt_print_data_section(data_decl_t *data){
    char *data_type;
    int64_t value;
    int i;
    imt_data_kind_t kind = data->kind;
    fprintf(stderr, "< %s > ", data->label);

    switch (kind)
    {
        case kind_data_byte:
            data_type = "< b >";
            break;

        case kind_data_int:
            data_type = "< l >";
            break;

        case kind_data_pointer:
            data_type = "< q >";
            break;
    }
    
    AL_FOR_EACH(int64_t, value, data->values, i)
    {
        fprintf(stderr, "\n    %s %ld", data_type, value);
    }
 
    fprintf(stderr, "\n");

}

void imt_print_text_section(entry_t *entry){
    imt_entry_kind_t kind = entry->kind; 
    fprintf(stderr, "< %d > ", entry->id);

    switch (kind)
    {
        case kind_ent_instruction:
            fprintf(stderr, " < ");  
            ins_t *ins = entry->value.instruction;  
            imt_print_instruction(ins);
            break;

        case kind_ent_label:
            fprintf(stderr, "< LABEL > %s \n", entry->value.label);
            break;

        case kind_ent_comment:
            fprintf(stderr, "< COMMENT > %s \n", entry->value.comment); 
            break;
        case kind_ent_empty:
            break;
        
        case kind_ent_tag:
            switch (entry->value.tag)
            {
                case kind_tag_statement_start:
                    fprintf(stderr, " < TAG > < Statement Start >\n");
                    break; 
                    
                case kind_tag_func_start:
                    fprintf(stderr, " < TAG > < Function Start >\n");
                    break; 
                    
                case kind_tag_func_end:
                    fprintf(stderr, " < TAG > < Function End >\n");
                    break; 
            }
            break;
    }
}

void imt_print_instruction(ins_t *ins){
    imt_ins_kind_t kind = ins->kind; 

     switch (kind)
    {
        // binary with result
        case kind_ins_add:  
            fprintf(stderr, "ADD > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;

        case kind_ins_sub:
            fprintf(stderr, "SUB > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_mult:
            fprintf(stderr, "MUL > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_divide:
            fprintf(stderr, "DIV > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_greater:
            fprintf(stderr, "GRT > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_greater_eq:
            fprintf(stderr, "GEQ > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_lower:
            fprintf(stderr, "LOW > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_lower_eq:
            fprintf(stderr, "LEQ > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_equal:
            fprintf(stderr, "EQ > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_not_equal:
            fprintf(stderr, "NEQ > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;
        
        case kind_ins_modulo:
            fprintf(stderr, "MOD > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;

        case kind_ins_lea:
            fprintf(stderr, "LEA > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;

        case kind_ins_cond_jump:
            fprintf(stderr, "COND_JMP > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.cond_jump.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.cond_jump.dst);            
            fprintf(stderr, "> < ");
            fprintf(stderr, "%s", ins->input.cond_jump.label);
            fprintf(stderr, ">");
            break;
            
        // binary
        case kind_ins_assign:
            fprintf(stderr, "ASSIGN > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);            
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;

        case kind_ins_cond_assign:
            fprintf(stderr, "COND_ASS > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.bin_args.src);
            fprintf(stderr, "> <");
            imt_print_argument(ins->input.bin_args.dst);
            fprintf(stderr, ">");
            break;

        // unary
        case kind_ins_increment:
            fprintf(stderr, "INC > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.unary_arg);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_decrement:
            fprintf(stderr, "DEC > ");
            imt_print_argument(ins->input.unary_arg);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_push:
            fprintf(stderr, "PUSH > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.unary_arg);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_pop:
            fprintf(stderr, "POP > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.unary_arg);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_not:
            fprintf(stderr, "NOT > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.unary_arg);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_neg:
            fprintf(stderr, "NEG > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.unary_arg);
            fprintf(stderr, ">");
            break;
            
        case kind_ins_jump:
            fprintf(stderr, "JMP > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.unary_arg);
            fprintf(stderr, ">");
            break;

        case kind_ins_call:
            fprintf(stderr, "CALL > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.unary_arg);
            fprintf(stderr, ">");
            break;

        case kind_ins_arithmetic_lshift:
            fprintf(stderr, "SAL > ");
            fprintf(stderr, "<");
            imt_print_argument(ins->input.unary_arg);
            fprintf(stderr, ">");
            break;
    }

    fprintf(stderr, "\n");
}

void imt_print_argument(arg_t *arg){
    imt_arg_kind_t kind = arg->kind; 

    switch (kind)
    {
        case kind_arg_address:
            imt_print_address(arg->value.address);
            break;

        case kind_arg_constant:
            fprintf(stderr, " ""%"PRId32 " " , arg->value.constant);
            break;

        case kind_arg_register:
            fprintf(stderr, " %s ", register_names[arg->value.reg]);
            break;

        case kind_arg_temporary:
            fprintf(stderr, " TMP %ld  ", arg->value.temporary);
            break;
    }    

}

void imt_print_address(address_t *addr){
    imt_address_kind_t kind = addr->kind; 

    switch (kind)
    {
        case kind_addr_dereference:
            fprintf(stderr, " ADDRESS_DEREFERENCE > "); 
            imt_print_argument(addr->value.deref_address);
            break;

        case kind_addr_label:
            fprintf(stderr, " ADDRESS_LABEL > < %s ", addr->value.label);
            break;

        case kind_addr_offset_derefference:
            fprintf(stderr, " OFFSET_DEREFFERENCE : ""%"PRId32" : ", addr->value.deref_offset.constant);
            imt_print_argument(addr->value.deref_offset.deref_address);
            break;

        case kind_addr_complex_derefference:
            fprintf(stderr," COMPLEX_DEREFFERENCE: " );
            fprintf(stderr, " ""%"PRId32 " : " ,addr->value.deref_complex.constant);

            imt_print_argument(addr->value.deref_complex.base);
            fprintf(stderr, " : ");
            imt_print_argument(addr->value.deref_complex.index);
            fprintf(stderr, " : %c ", addr->value.deref_complex.size );

            break;

    }

}

void imt_print_function_section(function_section_t *func){
    fprintf(stderr, "< FUNC NAME: %s > \n", func->name);
    entry_t *ent; 

    int i = 0;
    AL_FOR_EACH(entry_t *, ent, func->body ,i)
        imt_print_text_section(ent);
        
    fprintf(stderr, "\n");
}
