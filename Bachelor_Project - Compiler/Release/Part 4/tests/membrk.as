
.global _start
.section .text
_start:
    call meminit

    mov $10, %rax
    mov heap_currpos, %rdi
    mov data_size, %rsi
    shr $3, %rsi
    call memstore

	mov $60, %rax			# sys_exit
	mov $0, %rdi 			# no errors
	syscall

# function initializes the heap memory region 
.type meminit @function
meminit: 
    mov $12, %rax           # sys_brk
    mov $0, %rdi            # get start address
    syscall
    
    mov %rax, %rdi
    mov %rdi, heap_start
    mov %rdi, heap_currpos
    
    add data_size, %rdi
    mov %rdi, tospace
    
    mov $12, %rax           # sys_brk
    add data_size, %rdi     # allocate heap
    syscall
	
	cmp %rdi, %rax			# if not 0 in rax then error getting memory
	jne meminit_err
	
	ret

meminit_err: 
	mov $1, %rax 			# sys_write
	mov $1, %rdi			# fd stdout
	lea err1out, %rsi		# string to dest index
	mov $33, %rdx			# lenght of message 
	syscall 
	mov $60, %rax			# sys_exit
	mov $6, %rdi 			# out-of-memory err code 6
	syscall

# Effective function for copying bytes of any size
# %rdi dst address
# %rsi src address
# %rdx bytes to copy
.type memcopy @function
memcopy:    
    mov %rdx, %rcx
    shr $3, %rcx
    cld
    rep movsq
    
    ret

# Effective function for copying constants to array
# %rax constant
# %rdi start address
# %rsi element count
.type memstore @function
memstore:    
    mov %rsi, %rcx
    shr $3, %rcx
    cld
    rep stosq
    
    ret

.section .data
    string_start: .quad 0
    heap_start: .quad 0    
	heap_currpos: .quad 0
    tospace: .quad 0
    # Limit of 1 GB
    data_limit: .quad 0x20000000
	# Initial size of 4096 bytes, 
    # the usual size of a virtual memory page
    data_size: .quad 0x1000

    err1out:
    	.ascii "meminit: error allocating memory\n" # length 33
    err2out:
        .ascii "array out of bounds\n" # lenght 20
    err3out:
        .ascii "divide by zero\n" # lenght 15
    err4out:
        .ascii "non-positive length for allocating array\n" # length 40
    err5out:
        .ascii "use of null pointer\n" # lenght 20
    err6out:
    	.ascii "memory out of bounds\n" # lenght 21
    errlimit:
    	.ascii "1 GB limit exceeded\n" # lenght 20
    
.section .bbs
    .equ buffer_size, 30
    .equ r_header, 16
    .equ a_header, 16
    .lcomm buffer, buffer_size
