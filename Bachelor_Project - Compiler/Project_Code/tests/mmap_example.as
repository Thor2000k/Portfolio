.section .data
    alloc_data: .quad 0
    # 0x1000 = 4096 bytes, the usual size of a virtual memory page
    data_size: .quad 0x1000
    next: .quad 0

.global _start
.section .text
_start:
    # Allocate memory
    mov $9, %rax            # sys_mmap
    mov $0, %rdi            # kernal assigns start address
    mov data_size, %rsi     # bytes to allocate
    mov $3, %rdx            # can read and write
    mov $0x22, %r10         # private and anonymous flags set
    mov $-1, %r8            # fd, -1 = no file
    mov $0, %r9             # offset of 0
    syscall
    
    # rax has -1 on failure and otherwise the address 
    # of the allocated memory
    
    # save address of allocated memory
    mov %rax, alloc_data 
    mov %rax, next  
    
    # either use label directly
    movq $10, (next)        # save 10 (8 bytes) at start
    add $8, next            # increment next by 8 bytes
    
    # or move address to a register
    mov next, %rdi
    movq $20, (%rdi)        # save 20 (8 bytes) at next available
    add $8, %rdi            # increment next by 8 bytes  
    
    # save changes
    mov %rdi, next
    
    # deallocate the memory
    mov $11, %rax           # sys_munmap
    mov alloc_data, %rdi    # start of data
    mov data_size, %rsi     # size of data
    syscall
    
    # terminate
    mov $60, %rax           # sys_exit
    mov $0, %rdi            # no error
    syscall
    
    