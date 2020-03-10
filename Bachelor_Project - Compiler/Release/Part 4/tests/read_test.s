.extern scanf
.global main
.section .text
main:
    call read
    mov %rax, %rdi
    call int2string
    mov %rax, %rdi
    call write

terminate: 
    mov $60, %rax  # sys_exit 
    mov $0, %rdi   # No error 
    syscall 

# Compiler generated functions
# Convert int to string and saves in buffer
# %rdi int to convert
# %rax length of string
.type int2string, @function
int2string:
    push %r12
    push %r13
    push %r14

    # Local variables
    mov $buffer, %r12  # Current byte in buffer
    add $buffer_size, %r12
    sub $1, %r12
    mov $0, %r13                # Length of string
    mov $10, %rsi               # Constant divisor
    mov %rdi, %rax

    movb $0x0A, (%r12)          # add newline
    dec %r12
    inc %r13

    cmp $0, %rdi
    setl %r14b                  # Mark if negative
    jg is_positive
    je is_zero

    neg %rdi
    mov %rdi, %rax

is_positive:
    mov $0, %rdx

    div %rsi

    add $0x30, %rdx
    movb %dl, (%r12)
    dec %r12
    inc %r13

    cmp $0, %rax
    je int2string_end

    jmp is_positive

is_zero:
    movb $0x30, (%r12)
    dec %r12
    inc %r13

int2string_end:
    cmp $1, %r14
    jne int2string_not_negative

    movb $0x2D, (%r12)
    dec %r12
    inc %r13

int2string_not_negative:
    inc %r12
    movq %r12, string_start
    mov %r13, %rax

    pop %r14
    pop %r13
    pop %r12

    ret

# Write buffer to stdout
# %rdi size of buffer
.type write, @function
write:
    # move arg to proper registers
    mov %rdi, %rdx

    # write to stdout
    mov $1, %rax
    mov $1, %rdi
    mov string_start, %rsi
    syscall

    ret


# Function for reading integer stdin
# %rax a 64 bit signed interger from stdin
.type read @function
read:
    mov $0, %rax

    lea read_format, %rdi
    lea read_buffer, %rsi

    call scanf
    mov read_buffer, %rax
    ret

.section .data
    string_start: .quad 0
    read_buffer: .quad 0
    read_format: .ascii "%ld"   # length 3
    
.section .bbs
    .equ buffer_size, 30
    .lcomm buffer, buffer_size
