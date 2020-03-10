#include "assembler.h"
#include "memory.h"
#include "intermediate.h"
#include <stdio.h>

// Declaration of local methods
static void asm_print_newline(int indent_level);
static void asm_function(void *v_function);
static void asm_data_decl(void *v_data_decl);
static void asm_argument(arg_t *arg);
static void asm_entry(void *v_entry);
static void asm_instruction(ins_t *ins);
static int asm_ins_size(imt_ins_size_t size);
static void asm_argument(arg_t *arg);
static void asm_address(address_t *address);

void asm_initialize()
{
    // Enum to string
    asm_char_table[RAX] = malloc_string("%rax");
    asm_char_table[RCX] = malloc_string("%rcx");
    asm_char_table[RDX] = malloc_string("%rdx");
    asm_char_table[RBX] = malloc_string("%rbx");
    asm_char_table[RSI] = malloc_string("%rsi");
    asm_char_table[RDI] = malloc_string("%rdi");
    asm_char_table[RSP] = malloc_string("%rsp");
    asm_char_table[RBP] = malloc_string("%rbp");
    asm_char_table[R8] = malloc_string("%r8");
    asm_char_table[R9] = malloc_string("%r9");
    asm_char_table[R10] = malloc_string("%r10");
    asm_char_table[R11] = malloc_string("%r11");
    asm_char_table[R12] = malloc_string("%r12");
    asm_char_table[R13] = malloc_string("%r13");
    asm_char_table[R14] = malloc_string("%r14");
    asm_char_table[R15] = malloc_string("%r15");

    // Enum to imt arg
    asm_arg_table[RAX] = imt_make_argument(RAX, kind_arg_register);
    asm_arg_table[RCX] = imt_make_argument(RCX, kind_arg_register);
    asm_arg_table[RDX] = imt_make_argument(RDX, kind_arg_register);
    asm_arg_table[RBX] = imt_make_argument(RBX, kind_arg_register);
    asm_arg_table[RSI] = imt_make_argument(RSI, kind_arg_register);
    asm_arg_table[RDI] = imt_make_argument(RDI, kind_arg_register);
    asm_arg_table[RSP] = imt_make_argument(RSP, kind_arg_register);
    asm_arg_table[RBP] = imt_make_argument(RBP, kind_arg_register);
    asm_arg_table[R8] = imt_make_argument(R8, kind_arg_register);
    asm_arg_table[R9] = imt_make_argument(R9, kind_arg_register);
    asm_arg_table[R10] = imt_make_argument(R10, kind_arg_register);
    asm_arg_table[R11] = imt_make_argument(R11, kind_arg_register);
    asm_arg_table[R12] = imt_make_argument(R12, kind_arg_register);
    asm_arg_table[R13] = imt_make_argument(R13, kind_arg_register);
    asm_arg_table[R14] = imt_make_argument(R14, kind_arg_register);
    asm_arg_table[R15] = imt_make_argument(R15, kind_arg_register);

    // arg1 - arg6 for function calls
    asm_call_arguments[0] = RDI;
    asm_call_arguments[1] = RSI;
    asm_call_arguments[2] = RDX;
    asm_call_arguments[3] = RCX;
    asm_call_arguments[4] = R8;
    asm_call_arguments[5] = R9;

    // Unchanged from function call
    // asm_callee_save[0] = RBP;
    asm_callee_save[0] = R12;
    asm_callee_save[1] = R13;
    asm_callee_save[2] = R14;
    asm_callee_save[3] = R15;
    asm_callee_save[4] = RBX;
}

// Visual Studio pragma for folding in editor
#pragma region // Compiler generated build in functions
static void asm_buildin_write()
{
    if (!call_write->is_used)
        return;
   
    char *string =
    "# Convert int to string and saves in buffer\n"
    "# %rdi int to convert\n"
    "# %rax length of string\n"
    ".type int2string, @function\n"
    "int2string:\n"
    "    push %r12\n"
    "    push %r13\n"
    "    push %r14\n"
    "\n"
    "    # Local variables\n"
    "    mov $buffer, %r12  # Current byte in buffer\n"
    "    add $buffer_size, %r12\n"
    "    sub $1, %r12\n"
    "    mov $0, %r13                # Length of string\n"
    "    mov $10, %rsi               # Constant divisor\n"
    "    mov %rdi, %rax\n"
    "\n"
    "    movb $0x0A, (%r12)          # add newline\n"
    "    dec %r12\n"
    "    inc %r13\n"
    "\n"
    "    cmp $0, %rdi\n"
    "    setl %r14b                  # Mark if negative\n"
    "    jg is_positive\n"
    "    je is_zero\n"
    "\n"
    "    neg %rdi\n"
    "    mov %rdi, %rax\n"
    "\n"
    "is_positive:\n"
    "    mov $0, %rdx\n"
    "\n"
    "    div %rsi\n"
    "\n"
    "    add $0x30, %rdx\n"
    "    movb %dl, (%r12)\n"
    "    dec %r12\n"
    "    inc %r13\n"
    "\n"
    "    cmp $0, %rax\n"
    "    je int2string_end\n"
    "\n"
    "    jmp is_positive\n"
    "\n"
    "is_zero:\n"
    "    movb $0x30, (%r12)\n"
    "    dec %r12\n"
    "    inc %r13\n"
    "\n"
    "int2string_end:\n"
    "    cmp $1, %r14\n"
    "    jne int2string_not_negative\n"
    "\n"
    "    movb $0x2D, (%r12)\n"
    "    dec %r12\n"
    "    inc %r13\n"
    "\n"
    "int2string_not_negative:\n"
    "    inc %r12\n"
    "    movq %r12, string_start\n"
    "    mov %r13, %rax\n"
    "\n"
    "    pop %r14\n"
    "    pop %r13\n"
    "    pop %r12\n"
    "\n"
    "    ret\n"
    "\n"
    "# Write buffer to stdout\n"
    "# %rdi size of buffer\n"
    ".type write, @function\n"
    "write:\n"
    "    # move arg to proper registers\n"
    "    mov %rdi, %rdx\n"
    "\n"
    "    # write to stdout\n"
    "    mov $1, %rax\n"
    "    mov $1, %rdi\n"
    "    mov string_start, %rsi\n"
    "    syscall\n"
    "\n"
    "    ret\n"
    "\n";

    printf("%s", string);
}

static void asm_buildin_getstatic()
{
    if (!call_get_static->is_used)
        return;

    char *string =
    "# Get Static Link after n jumps\n"
    "# %rdi number of jumps\n"
    "# %rbp start static Link\n"
    "# %rax \n"
    ".type get_static, @function\n"
    "get_static:\n"
    "    mov %rbp, %rax\n"
    "\n"
    "get_static_loop:\n"
    "    mov (%rax), %rax\n"
    "    dec %rdi\n"
    "    jg get_static_loop          # Jump if %rdi > 0\n"
    "\n"
    "    ret\n"
    "\n";


    printf("%s", string);
}

static void asm_buildin_memory()
{
    if (!(call_allocate_array->is_used || 
        call_allocate_record->is_used))
        return;

    char *string =
    "# function initializes the heap memory region \n"
    ".type meminit @function\n"
    "meminit: \n"
    "    mov $12, %rax               # sys_brk\n"
    "    mov $0, %rdi                # get start address\n"
    "    syscall\n"
    "    \n"
    "    mov %rax, %rdi\n"
    "    mov %rdi, heap_start\n"
    "    mov %rdi, heap_currpos\n"
    "    mov %rdi, lowspace\n"
    "    \n"
    "    add data_size, %rdi\n"
    "    mov %rdi, highspace\n"
    "    \n"
    "    mov $12, %rax               # sys_brk\n"
    "    add data_size, %rdi         # allocate heap\n"
    "    syscall\n"
    "	\n"
    "	cmp %rdi, %rax			    # if not equal then error getting memory\n"
    "	jne meminit_err\n"
    "	\n"
    "    # Make sure new memory is zero\n"
    "    mov $0, %rax\n"
    "    mov heap_start, %rdi\n"
    "    mov data_size, %rsi \n"
    "    shr $3, %rsi\n"
    "    call memstore\n"
    "    \n"
    "	ret\n"
    "\n"
    "meminit_err: \n"
    "	mov $1, %rax 			    # sys_write\n"
    "	mov $1, %rdi			    # fd stdout\n"
    "	lea err1out, %rsi		    # string to dest index\n"
    "	mov $33, %rdx			    # lenght of message \n"
    "	syscall \n"
    "	mov $60, %rax			    # sys_exit\n"
    "	mov $6, %rdi 			    # out-of-memory err code 6\n"
    "	syscall\n"
    "\n"
    "#function that checks if requested bytes of heap space can be aquired. \n"
    "#reqeusted space must be passed in %rdi\n"
    "#uses data_size and heap_currpos\n"
    "#returns 1(true) or 0(false) in %rax if there is enough/not enough space \n"
    "\n"
    ".type memcheck @function\n"
    "memcheck:    \n"
    "	mov data_size, %r8\n"
    "    add heap_start, %r8\n"
    "	sub heap_currpos, %r8	    # subtracting current position in the heap\n"
    "    \n"
    "    xor %rax, %rax\n"
    "	cmp %rdi, %r8			    # with the total size to get remaining space\n"
    "	setge %al		            # set rax (1 byte reg) to 1 if enough\n"
    "\n"
    "	ret\n"
    "\n"
    "# function for expanding heap space \n"
    "# We expand heap space by a factor 2 up to a limit of 1gb \n"
    "# size of memory in rdi\n"
    ".type memexpand @function\n"
    "memexpand:   \n"
    "    push %rbx\n"
    "    \n"
    "    mov heap_start, %rbx\n"
    "    cmp lowspace, %rbx\n"
    "    je memexpand_resize\n"
    "    \n"
    "    # move data to low-space\n"
    "    push %rdi\n"
    "    call garbagecollection\n"
    "    pop %rdi\n"
    "    mov heap_start, %rbx\n"
    "    \n"
    "memexpand_resize: \n"
    "    push %r12    \n"
    "	movq data_size, %r12\n"
    "    \n"
    "memexpand_loop:	\n"
    "	shl $1, %r12				# calculating new size \n"
    "	cmp data_limit, %r12 		# comparing new size with upper limit\n"
    "	jg memexpand_limit_err\n"
    "    \n"
    "    cmp %r12, %rdi              # keep expanding if we need more space\n"
    "    jg memexpand_loop\n"
    "\n"
    "    # new stuff    \n"
    "    mov %r12, %rdi\n"
    "    shl $1, %rdi \n"
    "    add %rbx, %rdi              # new brk address\n"
    "    \n"
    "    mov $12, %rax               # sys_brk\n"
    "    syscall\n"
    "    \n"
    "    cmp %rax, %rdi              # if not equal we could not expand heap\n"
    "    jne memexpand_err\n"
    "    \n"
    "    # Make sure new memory is zero\n"
    "    mov $0, %rax\n"
    "    mov highspace, %rdi\n"
    "    mov %r12, %rsi\n"
    "    sub data_size, %rsi     \n"
    "    shr $3, %rsi\n"
    "    call memstore\n"
    "    \n"
    "    movq %r12, data_size\n"
    "    add %rbx, %r12\n"
    "    mov %r12, highspace\n"
    "    \n"
    "    pop %r12\n"
    "    pop %rbx\n"
    "    \n"
    "	ret\n"
    "\n"
    "memexpand_err:\n"
    "	mov $1, %rax 			    # sys_write\n"
    "	mov $1, %rdi			    # fd stdout\n"
    "	lea err6out, %rsi		    # string to dest index\n"
    "	mov $21, %rdx			    # lenght of message \n"
    "	syscall \n"
    "	mov $60, %rax			    # sys_exit\n"
    "	mov $6, %rdi 			    # out-of-memory err code 6\n"
    "	syscall\n"
    "    \n"
    "memexpand_limit_err:\n"
    "	mov $1, %rax 			    # sys_write\n"
    "	mov $1, %rdi			    # fd stdout\n"
    "	lea errlimit, %rsi		    # string to dest index\n"
    "	mov $20, %rdx			    # lenght of message \n"
    "	syscall \n"
    "	mov $60, %rax			    # sys_exit\n"
    "	mov $6, %rdi 			    # out-of-memory err code 6\n"
    "	syscall\n"
    "\n";

    printf("%s", string);

    string =
    "# Function for removing garbage in heap\n"
    ".type garbagecollection @function\n"
    "garbagecollection: \n"
    "\n"
    "    mov lowspace, %rsi\n"
    "    mov %rsi, %rdi\n"
    "    mov highspace, %rax\n"
    "    \n"
    "    cmp heap_start, %rax            # if low-space will be to-space, swap\n"
    "    cmove %rax, %rdi\n"
    "    cmove %rsi, %rax\n"
    "    \n"
    "    push %rdi\n"
    "    \n"
    "    # set to-space\n"
    "    movq %rax, heap_start\n"
    "    movq %rax, heap_currpos\n"
    "    \n"
    "    # move from-space -> to-space\n"
    "    call iterate_stack              # add from stack\n"
    "    call iterate_heap               # scan to-space\n"
    "    pop %rdi                        # get old start address\n"
    "    mov data_size, %rsi\n"
    "    call iterate_temps              # change temporary results\n"
    "        \n"
    "    # make sure unused space is 0\n"
    "    mov $0, %rax\n"
    "    mov heap_currpos, %rdi\n"
    "    mov heap_start, %rsi\n"
    "    add data_size, %rsi\n"
    "    sub %rdi, %rsi \n"
    "    shr $3, %rsi\n"
    "    call memstore\n"
    "    \n"
    "    ret    \n"
    "\n"
    "# function for allocating memory on the \"heap\"\n"
    "# size of memory requested be passed in %rdi\n"
    "# this is needed to store the metadata\n"
    "# returns adress to start of memory in %rax\n"
    "\n"
    ".type memalloc @function\n"
    "memalloc: \n"
    "	push %rdi						# pushing rdi to make sure value is unmodified\n"
    "    \n"
    "    cmp data_size, %rdi             # check if impossible to\n"
    "    jg memalloc_expand              # fit in current space\n"
    "    \n"
    "	call memcheck          			# Calling memcheck with argument in %rdi \n"
    "									# returns 1 / 0 in rax \n"
    "\n"
    "	cmp $1, %rax					# comparing result from memcheck with true\n"
    "	je memalloc_finalize\n"
    "\n"
    "	# lav garbage collection hvis fejler forøg memory her\n"
    "    call garbagecollection\n"
    "    \n"
    "    movq (%rsp), %rdi\n"
    "    call memcheck\n"
    "    \n"
    "    cmp $1, %rax\n"
    "    je memalloc_finalize    \n"
    "    \n"
    "memalloc_expand:\n"
    "    # minimum required size of new heap\n"
    "    movq heap_currpos, %rdi\n"
    "    subq heap_start, %rdi           # current bytes being used\n"
    "    addq (%rsp), %rdi               # min bytes needed\n"
    "	call memexpand         			# will exit program on failure \n"
    "\n"
    "memalloc_finalize: \n"
    "	pop %rdi						# popping allocation size\n"
    "	movq heap_currpos, %rax			# start of allocated memory, this value will be returned\n"
    "	add %rdi, heap_currpos 			# adding allocated size \n"
    "\n"
    "	ret\n"
    "\n";
    
    printf("%s", string);

    string =
    "# Iterate all pointer variables on the stack\n"
    "# Local\n"
    "# %rdi Static link of current frame\n"
    "# %r12 Content of variable on stack\n"
    "# %r13 Address of offset_table\n"
    "# %r14 Count in offset_table\n"
    ".type iterate_stack, @function\n"
    "iterate_stack:\n"
    "    push %rbp\n"
    "    push %r12\n"
    "    push %r13\n"
    "    push %r14\n"
    "    push %r15\n"
    "\n"
    "    mov %rbp, %rdi\n"
    "    \n"
    "get_meta:\n"
    "    lea offset_table(%rip), %r13    # address of offset_table\n"
    "    addq -8(%rdi), %r13             # address of functions info\n"
    "\n"
    "    movq (%r13), %r14               # get number of variables\n"
    "\n"
    "next_var:\n"
    "    dec %r14\n"
    "    jl previous_frame               # jump if r14 is lower than 0\n"
    "\n"
    "    add $8, %r13                    # increment offset_table pointer\n"
    "    movq (%r13), %r15               # save offset in r15 \n"
    "    lea (%rdi, %r15, 1), %r12       # get address of the next variable\n"
    "\n"
    "    push %rdi\n"
    "    \n"
    "    movq (%r12), %rdi               # address to heap as first argument    \n"
    "    call memfromptr                 # call function for variable\n"
    "    movq %rax, (%r12)               # replace content with new address to heap\n"
    "    \n"
    "    pop %rdi    \n"
    "    \n"
    "    jmp next_var  \n"
    "\n"
    "previous_frame:\n"
    "    cmp $0, (%rdi)\n"
    "    cmovne 8(%rdi), %rdi\n"
    "    jne get_meta\n"
    "\n"
    "    # epilogue\n"
    "    pop %r15\n"
    "    pop %r14\n"
    "    pop %r13\n"
    "    pop %r12\n"
    "    pop %rbp\n"
    "    ret \n"
    "\n"
    "# Iterate all pointers on the heap\n"
    "# Local\n"
    "# %r12 scan, current address in heap\n"
    "# %r13 counter for loops\n"
    "# %r14 address to offset_table\n"
    ".type iterate_heap, @function\n"
    "iterate_heap:\n"
    "    push %r12\n"
    "    push %r13\n"
    "    push %r14\n"
    "    movq heap_start, %r12\n"
    "    \n"
    "next_memory:\n"
    "    cmp heap_currpos, %r12\n"
    "    je iterate_heap_end\n"
    "    \n"
    "    movq (%r12), %r13\n"
    "    \n"
    "    cmp $2, %r13\n"
    "    je iterate_array\n"
    "    \n"
    "    cmp $3, %r13\n"
    "    je iterate_record\n"
    "    \n"
    "    # cmp $0, %r13              # should never happen\n"
    "    # je iterate_heap_end\n"
    "    \n"
    "    # array of non-pointer values\n"
    "    movq 8(%r12), %r13\n"
    "    imul $8, %r13\n"
    "    add $a_header, %r12         # add header to current address\n"
    "    add %r13, %r12              # add array size to current address\n"
    "    jmp next_memory\n"
    "    \n"
    "iterate_array:\n"
    "    movq 8(%r12), %r13\n"
    "    add $a_header, %r12    \n"
    "   \n"
    "iterate_array_loop:\n"
    "    dec %r13\n"
    "    jl next_memory\n"
    "    \n"
    "    movq (%r12), %rdi\n"
    "    \n"
    "    # call function for variable\n"
    "    call memfromptr\n"
    "    \n"
    "    movq %rax, (%r12)\n"
    "    \n"
    "    add $8, %r12\n"
    "    jmp iterate_array_loop\n"
    "\n"
    "iterate_record:\n"
    "    movq 8(%r12), %r14\n"
    "    movq 8(%r14), %r13          # count in offset_table\n"
    "\n"
    "iterate_record_loop:\n"
    "    dec %r13\n"
    "    jl iterate_record_end\n"
    "    \n"
    "    mov %r12, %rdi\n"
    "    addq 16(%r14, %r13, 8), %rdi\n"
    "    push %rdi\n"
    "    \n"
    "    mov (%rdi), %rdi\n"
    "    \n"
    "    # call function for variable pointer\n"
    "    call memfromptr\n"
    "    \n"
    "    pop %rdi\n"
    "    movq %rax, (%rdi)\n"
    "    \n"
    "    jmp iterate_record_loop\n"
    "    \n"
    "iterate_record_end:\n"
    "    addq (%r14), %r12\n"
    "    add $r_header, %r12\n"
    "    \n"
    "    jmp next_memory\n"
    "    \n"
    "iterate_heap_end:\n"
    "    pop %r14\n"
    "    pop %r13\n"
    "    pop %r12\n"
    "    ret\n"
    "\n";
    
    printf("%s", string);

    string =
    "# Iterate temporary result registers\n"
    "# %rdi old from-space start\n"
    "# %rsi old size\n"
    ".type iterate_temps, @function\n"
    "iterate_temps:\n"
    "    push %rbx\n"
    "    push %r8\n"
    "    \n"
    "    # redirect %r12\n"
    "    xor %rbx, %rbx\n"
    "    \n"
    "    mov %r12, %r8\n"
    "    sub %rdi, %r8\n"
    "    jl iterate_temps_r13\n"
    "\n"
    "    sub %rsi, %r8\n"
    "    jge iterate_temps_r13\n"
    "     \n"
    "    movq (%r12), %r12\n"
    "\n"
    "iterate_temps_r13:\n"
    "    # redirect %r13\n"
    "    xor %rbx, %rbx\n"
    "    \n"
    "    mov %r13, %r8\n"
    "    sub %rdi, %r8\n"
    "    jl iterate_temps_r14\n"
    "\n"
    "    subq %rsi, %r8\n"
    "    jge iterate_temps_r14\n"
    "     \n"
    "    movq (%r13), %r13\n"
    "\n"
    "iterate_temps_r14:    \n"
    "    # redirect %r14\n"
    "    xor %rbx, %rbx\n"
    "    \n"
    "    mov %r14, %r8\n"
    "    sub %rdi, %r8\n"
    "    jl iterate_temps_r15\n"
    "\n"
    "    subq %rsi, %r8\n"
    "    jge iterate_temps_r15\n"
    "     \n"
    "    movq (%r14), %r14\n"
    " \n"
    "iterate_temps_r15:   \n"
    "    # redirect %r15\n"
    "    xor %rbx, %rbx\n"
    "    \n"
    "    mov %r15, %r8\n"
    "    sub %rdi, %r8\n"
    "    jl iterate_temps_end\n"
    "\n"
    "    subq %rsi, %r8\n"
    "    jge iterate_temps_end\n"
    "     \n"
    "    movq (%r15), %r15\n"
    "\n"
    "iterate_temps_end:    \n"
    "    pop %r8\n"
    "    pop %rbx\n"
    "    ret\n"
    "\n"
    "# Function for pointer on from-space\n"
    "# %rdi address in from-space\n"
    "# %rax new address on to-space\n"
    ".type memfromptr, @function\n"
    "memfromptr:\n"
    "    push %r12\n"
    "    push %r13\n"
    "    \n"
    "    xor %rax, %rax\n"
    "    cmp $0, %rdi\n"
    "    je memfromptr_end\n"
    "    \n"
    "    mov %rdi, %r13    \n"
    "    movq (%r13), %r12               # first value in header\n"
    "    \n"
    "    # Check if header is an address\n"
    "    cmp $3, %r12\n"
    "    cmovg %r12, %rax\n"
    "    jg memfromptr_end               # header is not an id, so must be an address\n"
    "    \n"
    "    call memsize    \n"
    "    push %rax\n"
    "    \n"
    "    # copy from-space to to-space\n"
    "    movq heap_currpos, %rdi         # dst address\n"
    "    mov %r13, %rsi                  # src address\n"
    "    mov %rax, %rdx                  # bytes to copy\n"
    "    call memcopy\n"
    "    \n"
    "    # update header on from-space\n"
    "    movq heap_currpos, %rax\n"
    "    movq %rax, (%r13)\n"
    "    \n"
    "    # update next in to-space\n"
    "    pop %r12\n"
    "    addq %r12, heap_currpos\n"
    "    \n"
    "memfromptr_end:\n"
    "    pop %r13\n"
    "    pop %r12\n"
    "    \n"
    "    ret\n"
    "\n"
    "# function for getting size of an entry on heap\n"
    "# %rdi address of entry\n"
    "# %rax size in bytes\n"
    ".type memsize, @function\n"
    "memsize:\n"
    "    push %r12\n"
    "    movq (%rdi), %r12\n"
    "    \n"
    "    xor %rax, %rax\n"
    "    cmp $0, %r12\n"
    "    je memsize_end\n"
    "    \n"
    "    movq 8(%rdi), %rax\n"
    "    \n"
    "    cmp $3, %r12\n"
    "    je memsize_record\n"
    "    \n"
    "    # else array    \n"
    "    imul $8, %rax\n"
    "    add $a_header, %rax\n"
    "    jmp memsize_end\n"
    "    \n"
    "memsize_record:\n"
    "    movq (%rax), %rax\n"
    "    add $r_header, %rax\n"
    "    \n"
    "memsize_end:\n"
    "    pop %r12\n"
    "    ret\n"
    "\n"
    "# Effective function for copying bytes of any size\n"
    "# %rdi dst address\n"
    "# %rsi src address\n"
    "# %rdx bytes to copy\n"
    ".type memcopy @function\n"
    "memcopy:    \n"
    "    mov %rdx, %rcx\n"
    "    # shr $3, %rcx\n"
    "    cld\n"
    "    rep movsb\n"
    "    \n"
    "    ret\n"
    "\n"
    "# Effective function for copying constants to array\n"
    "# %rax constant\n"
    "# %rdi start address\n"
    "# %rsi element count\n"
    ".type memstore @function\n"
    "memstore:    \n"
    "    mov %rsi, %rcx\n"
    "    cld\n"
    "    rep stosq\n"
    "    \n"
    "    ret\n"
    "\n";

    printf("%s", string);
}

static void asm_buildin_alloc_array()
{
    if (!call_allocate_array->is_used)
        return;
    
    char *string = 
    "# function for allocating array\n"
    "# %rdi elements\n"
    "# %rsi element size\n"
    "# %rdx array type, 2 = pointers, 1 otherwise\n"
    "# %rax return start address of array\n"
    ".type allocate_array @function\n"
    "allocate_array:\n"
    "    \n"
    "    cmp $0, %rdi\n"
    "    jle allocate_array_err\n"
    "        \n"
    "    # allocate space\n"
    "    push %rdi\n"
    "    push %rdx\n"
    "    imul $8, %rdi           # bytes for array\n"
    "    add $a_header, %rdi     # extra space for metadata\n"
    "    call memalloc\n"
    "    pop %rdx\n"
    "    pop %rdi\n"
    "    \n"
    "    # add metadata\n"
    "    movq %rdx, (%rax)       # array type\n"
    "    movq %rdi, 8(%rax)      # size of array\n"
    "    \n"
    "    ret\n"
    "\n"
    "allocate_array_err:\n"
    "    mov $1, %rax 			# sys_write\n"
    "	mov $1, %rdi			# fd stdout \n"
    "	lea err4out, %rsi		# string to dest index\n"
    "	mov $40, %rdx			# lenght of message \n"
    "	syscall     \n"
    "	mov $60, %rax			# sys_exit\n"
    "	mov $4, %rdi 			# array out of bounds\n"
    "	syscall\n"
    "\n";

    printf("%s", string);
}

static void asm_buildin_array_index()
{
    
    if (!(intermediate_add_check || call_array_index->is_used || 
        call_memcopyfrom->is_used))
        return;

    char *string =
    "# function for checking array index\n"
    "# %rdi start address\n"
    "# %rsi index\n"
    ".type array_index @function\n"
    "array_index:    \n"
    "    # Check address\n"
    "    cmp $0, %rdi\n"
    "    je address_null_err\n"
    "\n"
    "    # check index\n"
    "    cmp $0, %rsi\n"
    "    jl array_index_err\n"
    "    \n"
    "    cmp 8(%rdi), %rsi\n"
    "    jge array_index_err\n"
    "     \n"
    "    ret\n"
    "\n"
    "array_index_err:\n"
    "    mov $1, %rax 			# sys_write\n"
    "	mov $1, %rdi			# fd stdout \n"
    "	lea err2out, %rsi		# string to dest index\n"
    "	mov $20, %rdx			# lenght of message \n"
    "	syscall \n"
    "	mov $60, %rax			# sys_exit\n"
    "	mov $2, %rdi 			# array out of bounds\n"
    "	syscall\n"
    "\n";

    printf("%s", string);
}

static void asm_buildin_alloc_record()
{
    if (!call_allocate_record->is_used)
        return;
    
    char *string =
    "# function for allocating record\n"
    "# %rdi size of record in bytes\n"
    "# %rsi start of offset_table\n"
    "# %rax return start address of record\n"
    ".type allocate_record @function\n"
    "allocate_record:    \n"
    "    # get address for offset_table entry\n"
    "    lea offset_table(%rip), %rbx\n"
    "    add %rsi, %rbx \n"
    "    \n"
    "    push %rdi\n"
    "    add $r_header, %rdi\n"
    "    call memalloc\n"
    "    pop %rdi\n"
    "    \n"
    "    # add meta data\n"
    "    movq $3, (%rax)         # record type\n"
    "    movq %rbx, 8(%rax)      # address to offset_table\n"
    "    \n"
    "    ret\n"
    "\n";

    printf("%s", string);
}

static void asm_buildin_checks()
{
    if (!(intermediate_add_check || call_memcopyfrom->is_used))
        return;

    char *string = 
    "address_null_err:\n"
    "    mov $1, %rax 			# sys_write\n"
    "	mov $1, %rdi			# fd stdout \n"
    "	lea err5out, %rsi		# string to dest index\n"
    "	mov $20, %rdx			# lenght of message \n"
    "	syscall \n"
    "	mov $60, %rax			# sys_exit\n"
    "	mov $5, %rdi 			# null pointer used\n"
    "	syscall\n"
    "    \n"
    "divide_by_zero_err:\n"
    "    mov $1, %rax 			# sys_write\n"
    "	mov $1, %rdi			# fd stdout \n"
    "	lea err3out, %rsi		# string to dest index\n"
    "	mov $15, %rdx			# lenght of message \n"
    "	syscall \n"
    "	mov $60, %rax			# sys_exit\n"
    "	mov $3, %rdi 			# null pointer used\n"
    "	syscall\n"
    "\n";

    printf("%s", string);
}

static void asm_buildin_memcopyfrom()
{
    if (!call_memcopyfrom->is_used)
        return;

    char *string =
    "# Copy from index of one array to index of another array\n"
    "# %rdi dst start of array\n"
    "# %rsi src start of array\n"
    "# %rdx number of iterations\n"
    "# %rcx dst start index\n"
    "# %r8  src start index\n"
    ".type memcopyfrom @function\n"
    "memcopyfrom:\n"
    "    push %r12 \n"
    "    push %r13\n"
    "\n"
    "    mov %rdi, %r12\n"
    "    mov %rsi, %r13\n"
    "\n"
    "    # check dst interval\n"
    "    mov %rcx, %rsi\n"
    "    call array_index\n"
    "\n"
    "    add %rdx, %rsi\n"
    "    dec %rsi\n"
    "    call array_index\n"
    "\n"
    "    # check src interval\n"
    "    mov %r13, %rdi\n"
    "    mov %r8, %rsi\n"
    "    call array_index\n"
    "\n"
    "    add %rdx, %rsi\n"
    "    dec %rsi\n"
    "    call array_index\n"
    "\n"
    "    # set parameters for copying\n"
    "    mov %r12, %rdi\n"
    "    add $a_header, %rdi\n"
    "    imul $8, %rcx\n"
    "    add %rcx, %rdi\n"
    "\n"
    "    mov %r13, %rsi\n"
    "    add $a_header, %rsi\n"
    "    imul $8, %r8\n"
    "    add %r8, %rsi\n"
    "\n"
    "    imul $8, %rdx\n"
    "    call memcopy\n"
    "\n"
    "    pop %r13\n"
    "    pop %r12\n"
    "\n"
    "    ret\n"
    "\n";

    printf("%s", string);
}

static void asm_buildin_sqrt()
{
    if (!call_sqrt->is_used)
        return;

    char *string =
    "# Function for finding the floor of the squar root\n"
    "# Base 2 square root, based on:\n"
    "# https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Example_3\n"
    "# %rdi number to get square root of\n"
    "# %rax the square root rounded down\n"
    ".type sqrt @function\n"
    "sqrt:\n"
    "    # %rax  res\n"
    "    # %rdi  num\n"
    "    # %rsi  bit\n"
    "    # %rdx  bit + res\n"
    "    \n"
    "    movq $0, %rax\n"
    "    mov $1, %rsi\n"
    "    shl $62, %rsi               # bit 2^62\n"
    "\n"
    "sqrt_set_bit:\n"
    "    cmp %rdi, %rsi              # while bit < num\n"
    "    jge sqrt_find_num\n"
    "\n"
    "    shr $2, %rsi\n"
    "    jmp sqrt_set_bit\n"
    "\n"
    "sqrt_find_num:\n"
    "    cmp $0, %rsi                # while bit != 0\n"
    "    je  sqrt_end\n"
    "\n"
    "    mov %rax, %rdx\n"
    "    add %rsi, %rdx\n"
    "    \n"
    "    cmp %rdx, %rdi\n"
    "    jl  sqrt_else\n"
    "    \n"
    "    sub %rdx, %rdi\n"
    "    \n"
    "    shr $1, %rax\n"
    "    add %rsi, %rax\n"
    "    \n"
    "    jmp sqrt_if_end\n"
    "    \n"
    "sqrt_else:\n"
    "    shr $1, %rax\n"
    "    \n"
    "sqrt_if_end:\n"
    "    shr $2, %rsi\n"
    "\n"
    "    jmp sqrt_find_num\n"
    "\n"
    "sqrt_end:\n"
    "    ret\n"
    "\n";

    printf("%s", string);
}

static void asm_buildin_readstdin()
{
    if (!call_readstdin->is_used)
        return;

    char *string =
    "# Read 64 bit integer from stdin, skips non-int strings\n"
    "# %rax int from stdin, 0 as default\n"
    ".type readstdin @function\n"
    "readstdin:\n"
    "    # set default and try to read int\n"
    "    movq $0, read_buffer\n"
    "    mov $0, %rax\n"
    "    \n"
    "    lea read_format, %rdi\n"
    "    lea read_buffer, %rsi\n"
    "    call scanf\n"
    "\n"
    "    cmp $1, %rax\n"
    "    je readstdin_end\n"
    "\n"
    "    # could not read an int, so read next char\n"
    "    movq $0, read_buffer\n"
    "    mov $0, %rax\n"
    "\n"
    "    lea read_any, %rdi\n"
    "    lea read_buffer, %rsi\n"
    "    call scanf\n"
    "    cmp $1, %rax\n"
    "    je readstdin\n"
    "\n"
    "    # reached end of stdinput\n"
    "\n"
    "readstdin_end:\n"
    "    movq read_buffer, %rax\n"
    "    ret\n"
    "\n";

    printf("%s", string);
}

static void asm_buildin_data()
{
    int data_is_used = call_write->is_used;
    int mem_is_used = call_allocate_array->is_used || call_allocate_record->is_used;

    data_is_used = data_is_used || mem_is_used;
    data_is_used = data_is_used || call_readstdin->is_used;
    data_is_used = data_is_used || intermediate_add_check;

    if (data_is_used)
        printf(".section .data\n");

    if (call_write->is_used)
        printf("    string_start: .quad 0\n");
 
    if (mem_is_used)
    {
        char *string =
        "    heap_start: .quad 0    \n"
        "	 heap_currpos: .quad 0\n"
        "    lowspace: .quad 0\n"
        "    highspace: .quad 0\n"
        "    # Limit of 500 MB\n"
        "    data_limit: .quad 0x20000000\n"
        "	 # Initial size of 4096 bytes, \n"
        "    # the usual size of a virtual memory page\n"
        "    data_size: .quad 0x1000\n";

        printf("%s", string);
    }

    if (call_readstdin->is_used)
    {
        char *string =
        "    read_buffer: .quad 0\n"
        "    read_format: .ascii \"%ld\\0\"   # length 3\n"
        "    read_any:    .ascii \"%c\\0\"\n";

        printf("%s", string);
    }

    if (intermediate_add_check || mem_is_used || call_memcopyfrom->is_used)
    {
        char *string =
        "\n"
        "    err1out:\n"
        "    	.ascii \"meminit: error allocating memory\\n\" # length 33\n"
        "    err2out:\n"
        "        .ascii \"array out of bounds\\n\"  # lenght 20\n"
        "    err3out:\n"
        "        .ascii \"divide by zero\\n\"       # lenght 15\n"
        "    err4out:\n"
        "        .ascii \"non-positive length for allocating array\\n\" # length 40\n"
        "    err5out:\n"
        "        .ascii \"use of null pointer\\n\"  # lenght 20\n"
        "    err6out:\n"
        "    	.ascii \"memory out of bounds\\n\" # lenght 21\n";
        
        printf("%s", string);
    }

    if (mem_is_used)
    {
        printf("    errlimit:\n");
        printf("    	.ascii \"1 GB limit exceeded\\n\"  # lenght 20\n");

    }

    printf("\n");

    if (call_write->is_used || mem_is_used)
    {
        printf(".section .bbs\n");
        if (call_write->is_used)
        {
            printf("    .equ buffer_size, 30\n");
            printf("    .lcomm buffer, buffer_size\n");    
        }
        if (mem_is_used)
        {
            printf("    .equ r_header, 16\n");
            printf("    .equ a_header, 16\n");
        }
    }
}

static void asm_compiler_generated()
{
    asm_print_newline(INDENT_EMPTY);
    printf("# Compiler generated functions\n");

    asm_print_newline(INDENT_EMPTY);
    
    asm_buildin_memory();
    asm_buildin_memcopyfrom();
    asm_buildin_alloc_array();
    asm_buildin_alloc_record();
    asm_buildin_array_index();
    asm_buildin_checks();
    asm_buildin_getstatic();
    asm_buildin_sqrt();
    asm_buildin_write();
    asm_buildin_readstdin();
    asm_buildin_data();
}
#pragma endregion

void asm_list(array_list *list, void (*asm_func)(void *))
{
    if (list == NULL)
    {
        return;
    }

    void *element;
    int i;

    AL_FOR_EACH(void *, element, list, i)
        asm_func(element);
}

static void asm_print_newline(int indent_level)
{
    printf("\n%*s", indent_level * 4, "");
}

void asm_program(program_t *program)
{
    if (program->data_section->size > 0)
    {
        printf(".section .data");
        asm_list(program->data_section, &asm_data_decl);
        asm_print_newline(INDENT_EMPTY);
        asm_print_newline(INDENT_DIRECTIVE);
    }

    printf(".global main");

    asm_print_newline(INDENT_DIRECTIVE);
    printf(".section .text");

    asm_print_newline(INDENT_LABEL);
    printf("main:");

    // print main scope code
    asm_list(program->text_section, &asm_entry);
    
    asm_print_newline(INDENT_EMPTY);
    asm_print_newline(INDENT_INS);
    printf("mov %%rbp, %%rsp");
    asm_print_newline(INDENT_INS);
    printf("pop %%rbp");
    asm_print_newline(INDENT_INS);
    printf("ret");

    // print functions
    asm_list(program->functions, &asm_function);

    asm_compiler_generated();
}

static void asm_function(void *v_function)
{
    function_section_t *function = v_function;

    asm_print_newline(INDENT_DIRECTIVE);
    printf(".type %s, @function", function->name);

    asm_list(function->body, &asm_entry);
    
    asm_print_newline(INDENT_INS);
    printf("ret\n");
}

static void asm_data_decl(void *v_data_decl)
{
    data_decl_t *data_decl = v_data_decl;
    
    asm_print_newline(INDENT_DECLARATION);
    printf("%s: ", data_decl->label);

    int64_t value;
    char *data_type;
    int i;
       
    switch (data_decl->kind)
    {
        case kind_data_byte:
            data_type = ".byte";
            break;

        case kind_data_int:
            data_type = ".int";
            break;

        case kind_data_pointer:
            data_type = ".quad";
            break;
    }

    AL_FOR_EACH(int64_t, value, data_decl->values, i)
    {
        asm_print_newline(INDENT_INS);

        printf("%s %ld", data_type, value);
    }

}

static void asm_entry(void *v_entry)
{
    entry_t *entry = v_entry;
    arg_t *arg;
    int i;

    switch (entry->kind)
    {
        case kind_ent_comment:
            asm_print_newline(INDENT_INS);
            printf("# %s", entry->value.comment);
            break;

        case kind_ent_empty: 
            asm_print_newline(INDENT_EMPTY);
            break;

        case kind_ent_label:
            asm_print_newline(INDENT_LABEL);
            printf("%s:", entry->value.label);
            break;

        case kind_ent_instruction:
            asm_instruction(entry->value.instruction);
            break;    

        case kind_ent_tag:
            switch (entry->value.tag)
            {
                case kind_tag_statement_start:
                    break;
                
                case kind_tag_func_start:
                    AL_FOR_EACH(arg_t *, arg, entry->extra_data, i)
                    {
                        asm_print_newline(INDENT_INS);
                        printf("push  ");
                        asm_argument(arg);
                    }
                    break;

                case kind_tag_func_end:
                    AL_FOR_EACH_REVERSE(arg_t *, arg, entry->extra_data, i)
                    {
                        asm_print_newline(INDENT_INS);
                        printf("pop  ");
                        asm_argument(arg);
                    }
                    break;
            }
            break;
    }
}

static void asm_instruction(ins_t *ins)
{
    asm_print_newline(INDENT_INS);
    int mem_is_used;

    switch (ins->kind)
    {
        // Binary with result
        case kind_ins_add:
            MNEMONIC("add", ins, bin_args.src, bin_args.dst)
            break;

        case kind_ins_sub:
            MNEMONIC("sub", ins, bin_args.src, bin_args.dst)
            break;

        case kind_ins_mult:
            MNEMONIC("imul", ins, bin_args.src, bin_args.dst)
            break;

        case kind_ins_divide:            
            printf("mov%c ", (char)ins->size);
            asm_argument(ins->input.bin_args.dst);
            printf(", %%rax");

            asm_print_newline(INDENT_INS);
            printf("push %%rdx");

            asm_print_newline(INDENT_INS);
            printf("cqto");

            asm_print_newline(INDENT_INS);
            printf("idiv%c ", (char)ins->size);
            asm_argument(ins->input.bin_args.src);

            asm_print_newline(INDENT_INS);
            printf("pop %%rdx");

            asm_print_newline(INDENT_INS);
            printf("movq %%rax, ");
            asm_argument(ins->input.bin_args.dst);
            break;

        case kind_ins_greater:
            INS_COMPARE("setg", ins)
            break;

        case kind_ins_greater_eq:
            INS_COMPARE("setge", ins)
            break;

        case kind_ins_lower:
            INS_COMPARE("setl", ins)
            break;

        case kind_ins_lower_eq:            
            INS_COMPARE("setle", ins)
            break;

        case kind_ins_equal:
            INS_COMPARE("sete", ins)
            break;

        case kind_ins_not_equal:
            INS_COMPARE("setne", ins)
            break;

        case kind_ins_modulo:            
            printf("mov%c ", (char)ins->size);
            asm_argument(ins->input.bin_args.dst);
            printf(", %%rax");

            asm_print_newline(INDENT_INS);
            printf("push %%rdx");

            asm_print_newline(INDENT_INS);
            printf("cqto");

            asm_print_newline(INDENT_INS);
            printf("idiv%c ", (char)ins->size);
            asm_argument(ins->input.bin_args.src);

            asm_print_newline(INDENT_INS);
            printf("movq %%rdx, ");
            asm_argument(ins->input.bin_args.dst);

            asm_print_newline(INDENT_INS);
            printf("pop %%rdx");
            break;

        case kind_ins_cond_jump:
            MNEMONIC("cmp", ins, cond_jump.src, 
                cond_jump.dst);
            
            asm_print_newline(INDENT_INS);
            printf("je %s", ins->input.cond_jump.label);
            break;

        // Binary
        case kind_ins_assign:                
            MNEMONIC("mov", ins, bin_args.src, bin_args.dst)
            break;

        case kind_ins_cond_assign:                
            MNEMONIC("cmovl", ins, bin_args.src, bin_args.dst)

            break;

        case kind_ins_lea:
            MNEMONIC("lea", ins, bin_args.src, bin_args.dst)

            break;

        // Unary
        case kind_ins_increment:
            printf("inc%c ", (char)ins->size);
            asm_argument(ins->input.unary_arg);
            break;

        case kind_ins_decrement:
            printf("dec%c ", (char)ins->size);
            asm_argument(ins->input.unary_arg);
            break;

        case kind_ins_push:
            printf("push%c ", (char)ins->size);
            asm_argument(ins->input.unary_arg);
            break;

        case kind_ins_pop:
            printf("pop%c ", (char)ins->size);
            asm_argument(ins->input.unary_arg);            
            break;

        case kind_ins_not:
            printf("not%c ", (char)ins->size);
            asm_argument(ins->input.unary_arg);
            break;

        case kind_ins_neg:
            printf("neg%c ", (char)ins->size);
            asm_argument(ins->input.unary_arg);
            break;

        case kind_ins_call:
            // Check if meminit call should be added 
            if (ins == call_meminit->function)
            {
                mem_is_used = call_allocate_array->is_used || 
                    call_allocate_record->is_used;

                if (!mem_is_used)
                    return;
            }

            printf("call ");
            asm_argument(ins->input.unary_arg);
            break;

        case kind_ins_jump:
            printf("jmp ");
            asm_argument(ins->input.unary_arg);
            break;

        case kind_ins_arithmetic_lshift:
            printf("sal%c $1, ", (char)ins->size);
            asm_argument(ins->input.unary_arg);

        break;

    }
}

static int asm_ins_size(imt_ins_size_t size)
{
    switch (size)
    {
        case kind_byte1:
            return 1;
            break;

        case kind_byte2:
            return 2;
            break;

        case kind_byte4:
            return 4;
            break;  

        case kind_byte8:
            return 8;
            break;

        case kind_bytes_not_set:
            return -1;
            break;          
    }

    return -1;
}

static void asm_argument(arg_t *arg)
{
    if (arg == NULL)
    {
        printf("NULL_ARG");
        return;
    }

    switch (arg->kind)
    {
        case kind_arg_constant:
            printf("$%d", arg->value.constant);
            break;

        case kind_arg_register:
            printf("%s", asm_char_table[arg->value.reg]);
            break;

        case kind_arg_address:
            asm_address(arg->value.address);
            break;    
        
        case kind_arg_temporary:
            // Shouldn't happen here.
            printf("t%ld", arg->value.temporary);
            break;
    }
}

static void asm_address(address_t *address)
{
    switch (address->kind)
    {
        case kind_addr_label:
            printf("%s", address->value.label);
            break;
    
        case kind_addr_dereference:
            printf("(");
            asm_argument(address->value.deref_address);
            printf(")");
            break;

        case kind_addr_offset_derefference:
            printf("%d(", address->value.deref_offset.constant);
            asm_argument(address->value.deref_offset.deref_address);
            printf(")");
            break;

        case kind_addr_complex_derefference:
            printf("%d(", address->value.deref_complex.constant);
            asm_argument(address->value.deref_complex.base);
            printf(", ");
            asm_argument(address->value.deref_complex.index);
            printf(", %d)", asm_ins_size(address->value.deref_complex.size));
            break;            
    }
}
