array:
    .quad 1
    .quad 2
    .quad 4
    .quad 8

.global _start
.section .text
_start:
    # Preamble
    lea array(%rip), %rdi
    movq 16(%rdi), %rdi
    push %rdi
    call int2string
    mov %rax, %rdi
    call write

    pop %rdi

terminate: 
    mov $60, %rax  # sys_exit 
    mov $0, %rdi   # No error 
    syscall 
 


.section .data
    string_start: .quad 0

.section .bbs
    .equ buffer_size, 30
    .lcomm buffer, buffer_size

.section .text
.section .text
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
    mov $0, %r13       # Length of string
    mov $10, %rsi      # Constant divisor
    mov %rdi, %rax

    movb $0x0A, (%r12)  # add newline
    dec %r12
    inc %r13

    cmp $0, %rdi
    setl %r14b          # Mark if negative
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

# Get Static Link after n jumps
# %rdi number of jumps
# %rbp start static Link
# %rax 
.type get_static, @function
get_static:
    mov %rbp, %rax

get_static_loop:
    mov (%rax), %rax    
    dec %rdi        
    jne get_static_loop     # Jump if %rdi > 0

    ret
