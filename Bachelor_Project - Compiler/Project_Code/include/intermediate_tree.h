#ifndef __H_INTERMEDIATE_TREE__
#define __H_INTERMEDIATE_TREE__

#include "array_list.h"
#include "assembler.h"
#include "tree.h"
#include <stdint.h>
#include <stdlib.h>

// forward declaration
typedef struct _function function_t;

// Simpler way to make an immediate argument. 
#define MAKE_CONSTANT(Constant) imt_make_argument(Constant, kind_arg_constant)

// Simpler way to make an instruction with one argument.
#define MAKE_INS_UNARY(arg1, Kind, Size) \
    imt_make_instruction(arg1, NULL, NULL, Kind, Size)

// Simpler way to make an instruction with two arguments.
#define MAKE_INS_BINARY(arg1, arg2, Kind, Size) \
    imt_make_instruction(arg1, arg2, NULL, Kind, Size)

// Simpler way to make an argument that is dereferenced.
#define MAKE_DEREF(Arg) \
imt_make_argument( \
    (size_t)imt_make_address( \
        0, \
        Arg, \
        NULL, \
        kind_bytes_not_set, \
        kind_addr_dereference), \
    kind_arg_address)

// Simpler way to make an argument that is dereferenced with an offset.
#define MAKE_DEREF_OFF(Arg, Offset) \
imt_make_argument( \
    (size_t)imt_make_address( \
        Offset, \
        Arg, \
        NULL, \
        kind_bytes_not_set, \
        kind_addr_offset_derefference), \
    kind_arg_address)

// Simpler way to make a label.
#define MAKE_LABEL(Label) \
imt_make_argument( \
    (size_t)imt_make_address_label(Label), \
    kind_arg_address)

// Simpler way to make a label that is dereferenced.
#define MAKE_DEREF_LABEL(Label) \
MAKE_DEREF(MAKE_LABEL(Label))

typedef struct _program program_t;
typedef struct _imt_data_decl data_decl_t; 
typedef struct _imt_entry entry_t;
typedef struct _imt_ins ins_t;
typedef struct _imt_arg arg_t;
typedef struct _imt_address address_t;
typedef struct _function_section function_section_t;

// program
struct _program
{
    array_list *data_section;
    array_list *text_section;
    array_list *functions;
};

// declerations
typedef enum
{
    kind_data_byte = 'b',
    kind_data_int = 'l',
    kind_data_pointer = 'q'
} imt_data_kind_t;

struct _imt_data_decl
{
    char *label;
    imt_data_kind_t kind;
    array_list *values;
};

// entries
typedef enum 
{
    kind_ent_instruction,
    kind_ent_label,
    kind_ent_comment,
    kind_ent_empty,
    kind_ent_tag
} imt_entry_kind_t;

typedef enum
{
    kind_tag_func_start, 
    kind_tag_func_end, 
    kind_tag_statement_start
} imt_tag_kind_t;

typedef union
{
    ins_t *instruction;
    char *label;
    char *comment;
    imt_tag_kind_t tag;
} imt_entry_value_t;

struct _imt_entry
{
    int id;
    imt_entry_kind_t kind;
    imt_entry_value_t value;
    array_list *extra_data;
};

// instructions
typedef enum 
{
    // binary
    // making sure they have same value as exp counterpart in tree.h
    kind_ins_add = 0,
    kind_ins_sub = 1,
    kind_ins_mult = 2,
    kind_ins_divide = 3,
    kind_ins_greater = 4,
    kind_ins_greater_eq = 6,
    kind_ins_lower = 5,
    kind_ins_lower_eq = 7,
    kind_ins_equal = 8,
    kind_ins_not_equal = 9,
    kind_ins_modulo = 12,
    kind_ins_cond_jump,
    kind_ins_assign,
    kind_ins_cond_assign,
    kind_ins_lea,
    // unary
    kind_ins_increment,
    kind_ins_decrement,
    kind_ins_push,
    kind_ins_pop,
    kind_ins_not = 100,
    kind_ins_neg,
    kind_ins_jump,
    kind_ins_call,
    kind_ins_arithmetic_lshift
} imt_ins_kind_t;

typedef enum
{
    kind_bytes_not_set = ' ',
    kind_byte1 = 'b',
    kind_byte2 = 'w',
    kind_byte4 = 'l',
    kind_byte8 = 'q'
} imt_ins_size_t;

typedef union
{
    struct { arg_t *src; arg_t *dst; char *label;} cond_jump;
    struct { arg_t *src; arg_t *dst; } bin_args;
    arg_t *unary_arg;
} imt_ins_value_t;

struct _imt_ins
{
    imt_ins_size_t size;
    imt_ins_kind_t kind;
    imt_ins_value_t input;    
};

// arguments
typedef enum kind_arg
{
    kind_arg_address,
    kind_arg_constant,
    kind_arg_register,
    kind_arg_temporary
} imt_arg_kind_t;

typedef union
{
    address_t *address;
    int32_t constant;
    asm_registers_t reg;
    size_t temporary;
} imt_arg_value_t;

struct _imt_arg
{
    imt_arg_kind_t kind;
    imt_arg_value_t value;
};

// address
typedef enum
{
    kind_addr_dereference,
    kind_addr_label,
    kind_addr_offset_derefference,
    kind_addr_complex_derefference
} imt_address_kind_t;

typedef union
{
    arg_t *deref_address;
    char *label;
    struct { int32_t constant; arg_t *deref_address; } deref_offset;
    struct { int32_t constant; arg_t *base; arg_t *index; imt_ins_size_t size; } deref_complex;
} imt_address_value_t;

struct _imt_address
{
    imt_address_kind_t kind;
    imt_address_value_t value;
};

// function Section
struct _function_section
{
    int offset_table_start;
    char *name;
    char *end_label;
    function_t *asm_node;
    array_list *body;
};

/* creates the main node for the intermediate tree */
program_t *imt_make_program();

function_section_t *imt_make_function(function_t *asm_node, 
    char *name, char *end_label, int offset_table_start);
data_decl_t *imt_make_data_decl(char *label, int expected_count, 
    imt_data_kind_t kind);
entry_t *imt_make_entry(void *param, imt_entry_kind_t kind);
ins_t *imt_make_instruction(arg_t *src, arg_t *dst, char *label, imt_ins_kind_t kind, 
    imt_ins_size_t size);
arg_t *imt_make_argument(size_t param1, imt_arg_kind_t kind);
address_t *imt_make_address(int32_t offset, arg_t *base, 
    arg_t *index, imt_ins_size_t kind_size, imt_address_kind_t kind);
address_t *imt_make_address_label(char *label);

#endif
