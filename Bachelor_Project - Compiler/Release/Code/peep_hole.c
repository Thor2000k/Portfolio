#include "peep_hole.h"
#include "array_list.h"
#include "intermediate_tree.h"
#include "hash_table.h"
#include <string.h>

typedef enum
{
  kind_pat_no_match = 0,
  kind_pat_push_pop = 1,
  // is left or right constant
  kind_pat_mult_by_2_left = 2,
  kind_pat_mult_by_2_right = 3,
  kind_pat_mult_by_1_left = 4,
  kind_pat_mult_by_1_right = 5,
  kind_pat_add_by_1_left = 6,
  kind_pat_add_by_1_right = 7,
  kind_pat_mult_by_0_left = 8,
  kind_pat_mult_by_0_right = 9,
  kind_pat_redundant_assignment = 10,
  kind_pat_sub_by_1_right = 12,
  kind_pat_add_by_0_left = 13,
  kind_pat_add_by_0_right = 14,
  kind_pat_sub_by_0_left = 15,
  kind_pat_sub_by_0_right = 16,

} ph_pattern_kind_t;

entry_t *ph_replace_src(entry_t *line_1, entry_t *line_2)
{
  arg_t *line_2_src = line_2->value.instruction->input.bin_args.src; 
  line_1->value.instruction->input.bin_args.src = line_2_src;

  return line_1;
}

entry_t *ph_redundant_assignment(entry_t *line_1, entry_t *line_2)
{
  arg_t *line_2_dst = line_2->value.instruction->input.bin_args.dst; 
  line_1->value.instruction->input.bin_args.dst = line_2_dst;

  return line_1;
}

// Pattern 1
ph_pattern_kind_t ph_check_push_pop(entry_t *line_1, entry_t *line_2)
{
  if (line_1->kind != kind_ent_instruction || line_2->kind != kind_ent_instruction)
    return kind_pat_no_match;

  if (line_1->value.instruction->kind == kind_ins_push && 
    line_2->value.instruction->kind == kind_ins_pop)
    return kind_pat_push_pop;
  else
    return kind_pat_no_match;  
}

ph_pattern_kind_t ph_check_op_by_constant(entry_t *line_1, entry_t *line_2, 
  imt_ins_kind_t kind, int constant, ph_pattern_kind_t pattern_left, 
  ph_pattern_kind_t patter_right)
{
  if (line_1->kind != kind_ent_instruction || line_2->kind != kind_ent_instruction || 
    line_2->value.instruction->kind != kind)
    return kind_pat_no_match;

  arg_t *src = line_1->value.instruction->input.bin_args.src;
  int is_const = src->kind == kind_arg_constant && src->value.constant == constant;

  if (is_const)
    return pattern_left;

  src = line_2->value.instruction->input.bin_args.src;
  is_const = src->kind == kind_arg_constant && src->value.constant == constant;
  
  if (is_const)
    return patter_right;
  else 
    return kind_pat_no_match;
}

ph_pattern_kind_t ph_check_redundant_assign(entry_t *line_1, entry_t *line_2)
{
  if (line_1->kind != kind_ent_instruction || line_2->kind != kind_ent_instruction)
    return kind_pat_no_match;

  ins_t *ins_1 = line_1->value.instruction;
  ins_t *ins_2 = line_2->value.instruction;

  if (ins_1->kind != kind_ins_assign || ins_2->kind != kind_ins_assign)
    return kind_pat_no_match;
  else if (ins_1->input.bin_args.dst != ins_2->input.bin_args.src)
    return kind_pat_no_match;
  else if (ins_1->input.bin_args.src->kind == kind_arg_address &&
    ins_2->input.bin_args.dst->kind == kind_arg_address)
    return kind_pat_no_match;
  else
    return kind_pat_push_pop;  
}

ph_pattern_kind_t identify_pattern(entry_t *line_1, entry_t *line_2)
{
  // 0 indicates no pattern was found/applicable. 
  // If a pattern matches, set value accordingly and return patterern value
  ph_pattern_kind_t pattern = kind_pat_no_match; 

  // push followed by a pop
  pattern = ph_check_push_pop(line_1, line_2);
  if (pattern != kind_pat_no_match)
    return pattern;

  // Multiplication by 2
  pattern = ph_check_op_by_constant(line_1, line_2, kind_ins_mult, 2, 
    kind_pat_mult_by_2_left, kind_pat_mult_by_2_right);
  if (pattern != kind_pat_no_match)
    return pattern;

  // Multiplication by 1  
  pattern = ph_check_op_by_constant(line_1, line_2, kind_ins_mult, 1, 
    kind_pat_mult_by_1_left, kind_pat_mult_by_1_right);
  if (pattern != kind_pat_no_match)
    return pattern;

  // Addition of 1
  pattern = ph_check_op_by_constant(line_1, line_2, kind_ins_add, 1, 
    kind_pat_add_by_1_left, kind_pat_add_by_1_right);
  if (pattern != kind_pat_no_match)
    return pattern;

  // Multiplication by 0  
  pattern = ph_check_op_by_constant(line_1, line_2, kind_ins_mult, 0, 
    kind_pat_mult_by_0_left, kind_pat_mult_by_0_right);
  if (pattern != kind_pat_no_match)
    return pattern;

  // Addition of 0
  pattern = ph_check_op_by_constant(line_1, line_2, kind_ins_add, 0, 
    kind_pat_add_by_0_left, kind_pat_add_by_0_right);
  if (pattern != kind_pat_no_match)
    return pattern;

  // Subtraction of 0
  pattern = ph_check_op_by_constant(line_1, line_2, kind_ins_sub, 0, 
    kind_pat_sub_by_0_left, kind_pat_sub_by_0_right);
  if (pattern != kind_pat_no_match)
    return pattern;

  // Subtraction of 1
  pattern = ph_check_op_by_constant(line_1, line_2, kind_ins_sub, 1, 
    kind_pat_no_match, kind_pat_sub_by_1_right);
  
 return pattern;
}

array_list *destroy_redundant_code(array_list *asm_code)
{
  if (asm_code == NULL || asm_code->size < 2)
    return asm_code;

  entry_t *first_entry;
  entry_t *second_entry;

  int modified = 0;

  array_list *opt_code = AL_INIT_PTR(asm_code->size);
  int i = 0;
  
  AL_GET_PTR(first_entry, asm_code, i);
  i++;

  for (; i < asm_code->size; i++)
  {
    AL_GET_PTR(second_entry, asm_code, i);
    if (second_entry == NULL)
      break;
    
    ph_pattern_kind_t pattern = identify_pattern(first_entry, second_entry);
    modified = modified || pattern != kind_pat_no_match;    

    switch(pattern)
    {
      // Delete both
      case kind_pat_push_pop:
        i++;
        AL_GET_PTR(first_entry, asm_code, i);
        break;

      // Delete second
      case kind_pat_mult_by_1_right:
      case kind_pat_mult_by_0_left:
      case kind_pat_add_by_0_right:
      case kind_pat_sub_by_0_right:        
        break;
      
      // Replacement
      case kind_pat_mult_by_2_left:    
        first_entry = ph_replace_src(first_entry, second_entry);
        al_add(opt_code, &first_entry);

        second_entry->value.instruction = MAKE_INS_UNARY(
          second_entry->value.instruction->input.bin_args.dst, 
          kind_ins_arithmetic_lshift, kind_bytes_not_set);
      
        first_entry = second_entry;
        break;

      case kind_pat_mult_by_2_right:
        al_add(opt_code, &first_entry);

        second_entry->value.instruction = MAKE_INS_UNARY(
          second_entry->value.instruction->input.bin_args.dst, 
          kind_ins_arithmetic_lshift, kind_bytes_not_set);
        
        first_entry = second_entry;
        break;

      case kind_pat_mult_by_1_left:
      case kind_pat_mult_by_0_right:
      case kind_pat_add_by_0_left:
        first_entry = ph_replace_src(first_entry, second_entry);
        al_add(opt_code, &first_entry);
        i++;
        AL_GET_PTR(first_entry, asm_code, i);
        break;

      case kind_pat_add_by_1_left:
        first_entry = ph_replace_src(first_entry, second_entry);
        al_add(opt_code, &first_entry);

        first_entry = imt_make_entry(MAKE_INS_UNARY(
          second_entry->value.instruction->input.bin_args.dst, 
          kind_ins_increment, kind_bytes_not_set), kind_ent_instruction);     
        break;

      case kind_pat_add_by_1_right:
        al_add(opt_code, &first_entry);

        first_entry = imt_make_entry(MAKE_INS_UNARY(
          second_entry->value.instruction->input.bin_args.dst, 
          kind_ins_increment, kind_bytes_not_set), kind_ent_instruction);
        break;
        
      case kind_pat_redundant_assignment:
        first_entry = ph_redundant_assignment(first_entry, second_entry);
        al_add(opt_code, &first_entry);
        break;

      case kind_pat_sub_by_1_right:
        al_add(opt_code, &first_entry);
        
        first_entry = imt_make_entry(MAKE_INS_UNARY(
          second_entry->value.instruction->input.bin_args.dst, 
          kind_ins_decrement, kind_bytes_not_set), kind_ent_instruction);
        break;
        
      case kind_pat_sub_by_0_left:
        first_entry = ph_replace_src(first_entry, second_entry);
        al_add(opt_code, &first_entry);

        first_entry = imt_make_entry(MAKE_INS_UNARY(
          second_entry->value.instruction->input.bin_args.dst, 
          kind_ins_neg, kind_bytes_not_set), kind_ent_instruction);
        break;

      case kind_pat_no_match:
        al_add(opt_code, &first_entry);
        first_entry = second_entry;
        break;
      default:
        break;
    }
  }

  if (second_entry != NULL)
    al_add(opt_code, &second_entry);

  if (modified)
    opt_code = destroy_redundant_code(opt_code);

  return opt_code;
}

array_list *ph_optimize_code(array_list *assembly_code)
{
  array_list *optimized_code = destroy_redundant_code(assembly_code);
  return optimized_code;
}

void peep_hole_optimizer(program_t *p)
{ 
  p->text_section = ph_optimize_code(p->text_section);

  int i;
  function_section_t *function;

  AL_FOR_EACH(function_section_t *, function, p->functions, i)
  {
    function->body = ph_optimize_code(function->body);
  }
}

