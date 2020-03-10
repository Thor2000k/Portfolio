#ifndef __H_ASSEMBLER__
#define __H_ASSEMBLER__

// forward declaration
typedef struct _program program_t;
typedef struct _imt_arg arg_t;

// Indent level in assembler code when making a new line.
#define INDENT_DIRECTIVE 0
#define INDENT_LABEL 0
#define INDENT_EMPTY 0
#define INDENT_DECLARATION 1
#define INDENT_INS 1

// Prints a mnemonic to stdout.
#define MNEMONIC(Name, Ins, Arg1, Arg2) \
    printf("%s%c ", Name, (char)Ins->size); \
    asm_argument(Ins->input.Arg1); \
    printf(", "); \
    asm_argument(Ins->input.Arg2); \

// Prints a comparison instruction to stdout.
#define INS_COMPARE(Name, Ins) \
    MNEMONIC("cmp", Ins, bin_args.src, \
        bin_args.dst) \
    asm_print_newline(INDENT_INS); \
    printf("mov $0, "); \
    asm_argument(Ins->input.bin_args.dst); \
    asm_print_newline(INDENT_INS); \
    printf("%s ", Name); \
    asm_argument(Ins->input.bin_args.dst); \
    printf("b");

typedef enum _asm_registers
{
    RAX,
    RCX,
    RDX,
    RBX,
    RSI,
    RDI,
    RSP,
    RBP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15
} asm_registers_t;

char *asm_char_table[16];
arg_t *asm_arg_table[16];
asm_registers_t asm_call_arguments[6];
asm_registers_t asm_caller_save[10];
asm_registers_t asm_callee_save[6];

/* initializes the register arrays. */
void asm_initialize();

/* prints assembler file from intermediate program to stdout. */
void asm_program(program_t *program);

#endif
