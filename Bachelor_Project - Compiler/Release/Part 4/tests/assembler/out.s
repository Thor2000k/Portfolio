.section .data
    offset_table: 
    .quad 1
    .quad -32
    .quad 1
    .quad -16

.global main
.section .text
main:
    # Preamble
    push  %rbp
    push  $0
    movq %rsp, %rbp
    push  $0
    movq %rbp, %r9
    sub  $24, %rsp

    # Assigning default values
    movq $0, -16(%r9)
    movq $0, -24(%rbp)
    movq $0, -32(%r9)

    # Init memory
    call meminit

    # Main scope code
    movq $0, -16(%r9)

    # Function call start
    push  %r9
    movq %r9, %rax
    call f1_outOfMem
    pop  %r9
    movq %rax, %r12
    # Function call end
    movq %r12, -24(%rbp)

    mov %rbp, %rsp
    pop %rbp
    ret
.type f1_outOfMem, @function
f1_outOfMem:
    # Preamble
    push  %rbp
    push  %rax
    movq %rsp, %rbp
    push  $16
    movq %rbp, %r9
    sub  $8, %rsp

    # Assigning default values
    movq $0, -16(%rbp)

    # Function body

    movq $1, %rdi
    call get_static
    movq %rax, %r9

    movq -16(%r9), %rdi
    call int2string
    movq %rax, %rdi
    call write

    movq -16(%r9), %r12
    inc  %r12
    movq %r12, -16(%r9)

    movq $0, -32(%r9)
    movq $2000, %rdi
    movq $8, %rsi
    movq $1, %rdx
    call allocate_array
    movq %rax, -32(%r9)

    movq $0, -16(%rbp)
    movq $2000, %rdi
    movq $8, %rsi
    movq $1, %rdx
    call allocate_array
    movq %rax, -16(%rbp)


    # Function call start
    push  %r9
    movq %r9, %rax
    call f1_outOfMem
    pop  %r9
    movq %rax, %r12
    # Function call end
    movq %r12, %rax
    jmp end1_outOfMem
end1_outOfMem:
    movq %rbp, %rsp
    add  $8, %rsp
    pop  %rbp
    ret

# Compiler generated functions

# function initializes the heap memory region 
.type meminit @function
meminit: 
    mov $12, %rax               # sys_brk
    mov $0, %rdi                # get start address
    syscall
    
    mov %rax, %rdi
    mov %rdi, heap_start
    mov %rdi, heap_currpos
    mov %rdi, lowspace
    
    add data_size, %rdi
    mov %rdi, highspace
    
    mov $12, %rax               # sys_brk
    add data_size, %rdi         # allocate heap
    syscall
	
	cmp %rdi, %rax			    # if not equal then error getting memory
	jne meminit_err
	
    # Make sure new memory is zero
    mov $0, %rax
    mov heap_start, %rdi
    mov data_size, %rsi 
    shr $3, %rsi
    call memstore
    
	ret

meminit_err: 
	mov $1, %rax 			    # sys_write
	mov $1, %rdi			    # fd stdout
	lea err1out, %rsi		    # string to dest index
	mov $33, %rdx			    # lenght of message 
	syscall 
	mov $60, %rax			    # sys_exit
	mov $6, %rdi 			    # out-of-memory err code 6
	syscall

#function that checks if requested bytes of heap space can be aquired. 
#reqeusted space must be passed in %rdi
#uses data_size and heap_currpos
#returns 1(true) or 0(false) in %rax if there is enough/not enough space 

.type memcheck @function
memcheck:    
	mov data_size, %r8
    add heap_start, %r8
	sub heap_currpos, %r8	    # subtracting current position in the heap
    
    xor %rax, %rax
	cmp %rdi, %r8			    # with the total size to get remaining space
	setge %al		            # set rax (1 byte reg) to 1 if enough

	ret

# function for expanding heap space 
# We expand heap space by a factor 2 up to a limit of 1gb 
# size of memory in rdi
.type memexpand @function
memexpand:   
    push %rbx
    
    mov heap_start, %rbx
    cmp lowspace, %rbx
    je memexpand_resize
    
    # move data to low-space
    push %rdi
    call garbagecollection
    pop %rdi
    mov heap_start, %rbx
    
memexpand_resize: 
    push %r12    
	movq data_size, %r12
    
memexpand_loop:	
	shl $1, %r12				# calculating new size 
	cmp data_limit, %r12 		# comparing new size with upper limit
	jg memexpand_limit_err
    
    cmp %r12, %rdi              # keep expanding if we need more space
    jg memexpand_loop

    # new stuff    
    mov %r12, %rdi
    shl $1, %rdi 
    add %rbx, %rdi              # new brk address
    
    mov $12, %rax               # sys_brk
    syscall
    
    cmp %rax, %rdi              # if not equal we could not expand heap
    jne memexpand_err
    
    # Make sure new memory is zero
    mov $0, %rax
    mov highspace, %rdi
    mov %r12, %rsi
    sub data_size, %rsi     
    shr $3, %rsi
    call memstore
    
    movq %r12, data_size
    add %rbx, %r12
    mov %r12, highspace
    
    pop %r12
    pop %rbx
    
	ret

memexpand_err:
	mov $1, %rax 			    # sys_write
	mov $1, %rdi			    # fd stdout
	lea err6out, %rsi		    # string to dest index
	mov $21, %rdx			    # lenght of message 
	syscall 
	mov $60, %rax			    # sys_exit
	mov $6, %rdi 			    # out-of-memory err code 6
	syscall
    
memexpand_limit_err:
	mov $1, %rax 			    # sys_write
	mov $1, %rdi			    # fd stdout
	lea errlimit, %rsi		    # string to dest index
	mov $20, %rdx			    # lenght of message 
	syscall 
	mov $60, %rax			    # sys_exit
	mov $6, %rdi 			    # out-of-memory err code 6
	syscall

# Function for removing garbage in heap
.type garbagecollection @function
garbagecollection: 

    mov lowspace, %rsi
    mov %rsi, %rdi
    mov highspace, %rax
    
    cmp heap_start, %rax            # if low-space will be to-space, swap
    cmove %rax, %rdi
    cmove %rsi, %rax
    
    push %rdi
    
    # set to-space
    movq %rax, heap_start
    movq %rax, heap_currpos
    
    # move from-space -> to-space
    call iterate_stack              # add from stack
    call iterate_heap               # scan to-space
    pop %rdi                        # get old start address
    mov data_size, %rsi
    call iterate_temps              # change temporary results
        
    # make sure unused space is 0
    mov $0, %rax
    mov heap_currpos, %rdi
    mov heap_start, %rsi
    add data_size, %rsi
    sub %rdi, %rsi 
    shr $3, %rsi
    call memstore
    
    ret    

# function for allocating memory on the "heap"
# size of memory requested be passed in %rdi
# this is needed to store the metadata
# returns adress to start of memory in %rax

.type memalloc @function
memalloc: 
	push %rdi						# pushing rdi to make sure value is unmodified
    
    cmp data_size, %rdi             # check if impossible to
    jg memalloc_expand              # fit in current space
    
	call memcheck          			# Calling memcheck with argument in %rdi 
									# returns 1 / 0 in rax 

	cmp $1, %rax					# comparing result from memcheck with true
	je memalloc_finalize

	# lav garbage collection hvis fejler forøg memory her
    call garbagecollection
    
    movq (%rsp), %rdi
    call memcheck
    
    cmp $1, %rax
    je memalloc_finalize    
    
memalloc_expand:
    # minimum required size of new heap
    movq heap_currpos, %rdi
    subq heap_start, %rdi           # current bytes being used
    addq (%rsp), %rdi               # min bytes needed
	call memexpand         			# will exit program on failure 

memalloc_finalize: 
	pop %rdi						# popping allocation size
	movq heap_currpos, %rax			# start of allocated memory, this value will be returned
	add %rdi, heap_currpos 			# adding allocated size 

	ret

# Iterate all pointer variables on the stack
# Local
# %rdi Static link of current frame
# %r12 Content of variable on stack
# %r13 Address of offset_table
# %r14 Count in offset_table
.type iterate_stack, @function
iterate_stack:
    push %rbp
    push %r12
    push %r13
    push %r14
    push %r15

    mov %rbp, %rdi
    
get_meta:
    lea offset_table(%rip), %r13    # address of offset_table
    addq -8(%rdi), %r13             # address of functions info

    movq (%r13), %r14               # get number of variables

next_var:
    dec %r14
    jl previous_frame               # jump if r14 is lower than 0

    add $8, %r13                    # increment offset_table pointer
    movq (%r13), %r15               # save offset in r15 
    lea (%rdi, %r15, 1), %r12       # get address of the next variable

    push %rdi
    
    movq (%r12), %rdi               # address to heap as first argument    
    call memfromptr                 # call function for variable
    movq %rax, (%r12)               # replace content with new address to heap
    
    pop %rdi    
    
    jmp next_var  

previous_frame:
    cmp $0, (%rdi)
    cmovne 8(%rdi), %rdi
    jne get_meta

    # epilogue
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbp
    ret 

# Iterate all pointers on the heap
# Local
# %r12 scan, current address in heap
# %r13 counter for loops
# %r14 address to offset_table
.type iterate_heap, @function
iterate_heap:
    push %r12
    push %r13
    push %r14
    movq heap_start, %r12
    
next_memory:
    cmp heap_currpos, %r12
    je iterate_heap_end
    
    movq (%r12), %r13
    
    cmp $2, %r13
    je iterate_array
    
    cmp $3, %r13
    je iterate_record
    
    # cmp $0, %r13              # should never happen
    # je iterate_heap_end
    
    # array of non-pointer values
    movq 8(%r12), %r13
    imul $8, %r13
    add $a_header, %r12         # add header to current address
    add %r13, %r12              # add array size to current address
    jmp next_memory
    
iterate_array:
    movq 8(%r12), %r13
    add $a_header, %r12    
   
iterate_array_loop:
    dec %r13
    jl next_memory
    
    movq (%r12), %rdi
    
    # call function for variable
    call memfromptr
    
    movq %rax, (%r12)
    
    add $8, %r12
    jmp iterate_array_loop

iterate_record:
    movq 8(%r12), %r14
    movq 8(%r14), %r13          # count in offset_table

iterate_record_loop:
    dec %r13
    jl iterate_record_end
    
    mov %r12, %rdi
    addq 16(%r14, %r13, 8), %rdi
    push %rdi
    
    mov (%rdi), %rdi
    
    # call function for variable pointer
    call memfromptr
    
    pop %rdi
    movq %rax, (%rdi)
    
    jmp iterate_record_loop
    
iterate_record_end:
    addq (%r14), %r12
    add $r_header, %r12
    
    jmp next_memory
    
iterate_heap_end:
    pop %r14
    pop %r13
    pop %r12
    ret

# Iterate temporary result registers
# %rdi old from-space start
# %rsi old size
.type iterate_temps, @function
iterate_temps:
    push %rbx
    push %r8
    
    # redirect %r12
    xor %rbx, %rbx
    
    mov %r12, %r8
    sub %rdi, %r8
    jl iterate_temps_r13

    sub %rsi, %r8
    jge iterate_temps_r13
     
    movq (%r12), %r12

iterate_temps_r13:
    # redirect %r13
    xor %rbx, %rbx
    
    mov %r13, %r8
    sub %rdi, %r8
    jl iterate_temps_r14

    subq %rsi, %r8
    jge iterate_temps_r14
     
    movq (%r13), %r13

iterate_temps_r14:    
    # redirect %r14
    xor %rbx, %rbx
    
    mov %r14, %r8
    sub %rdi, %r8
    jl iterate_temps_r15

    subq %rsi, %r8
    jge iterate_temps_r15
     
    movq (%r14), %r14
 
iterate_temps_r15:   
    # redirect %r15
    xor %rbx, %rbx
    
    mov %r15, %r8
    sub %rdi, %r8
    jl iterate_temps_end

    subq %rsi, %r8
    jge iterate_temps_end
     
    movq (%r15), %r15

iterate_temps_end:    
    pop %r8
    pop %rbx
    ret

# Function for pointer on from-space
# %rdi address in from-space
# %rax new address on to-space
.type memfromptr, @function
memfromptr:
    push %r12
    push %r13
    
    xor %rax, %rax
    cmp $0, %rdi
    je memfromptr_end
    
    mov %rdi, %r13    
    movq (%r13), %r12               # first value in header
    
    # Check if header is an address
    cmp $3, %r12
    cmovg %r12, %rax
    jg memfromptr_end               # header is not an id, so must be an address
    
    call memsize    
    push %rax
    
    # copy from-space to to-space
    movq heap_currpos, %rdi         # dst address
    mov %r13, %rsi                  # src address
    mov %rax, %rdx                  # bytes to copy
    call memcopy
    
    # update header on from-space
    movq heap_currpos, %rax
    movq %rax, (%r13)
    
    # update next in to-space
    pop %r12
    addq %r12, heap_currpos
    
memfromptr_end:
    pop %r13
    pop %r12
    
    ret

# function for getting size of an entry on heap
# %rdi address of entry
# %rax size in bytes
.type memsize, @function
memsize:
    push %r12
    movq (%rdi), %r12
    
    xor %rax, %rax
    cmp $0, %r12
    je memsize_end
    
    movq 8(%rdi), %rax
    
    cmp $3, %r12
    je memsize_record
    
    # else array    
    imul $8, %rax
    add $a_header, %rax
    jmp memsize_end
    
memsize_record:
    movq (%rax), %rax
    add $r_header, %rax
    
memsize_end:
    pop %r12
    ret

# Effective function for copying bytes of any size
# %rdi dst address
# %rsi src address
# %rdx bytes to copy
.type memcopy @function
memcopy:    
    mov %rdx, %rcx
    # shr $3, %rcx
    cld
    rep movsb
    
    ret

# Effective function for copying constants to array
# %rax constant
# %rdi start address
# %rsi element count
.type memstore @function
memstore:    
    mov %rsi, %rcx
    cld
    rep stosq
    
    ret

# function for allocating array
# %rdi elements
# %rsi element size
# %rdx array type, 2 = pointers, 1 otherwise
# %rax return start address of array
.type allocate_array @function
allocate_array:
    
    cmp $0, %rdi
    jle allocate_array_err
        
    # allocate space
    push %rdi
    push %rdx
    imul $8, %rdi           # bytes for array
    add $a_header, %rdi     # extra space for metadata
    call memalloc
    pop %rdx
    pop %rdi
    
    # add metadata
    movq %rdx, (%rax)       # array type
    movq %rdi, 8(%rax)      # size of array
    
    ret

allocate_array_err:
    mov $1, %rax 			# sys_write
	mov $1, %rdi			# fd stdout 
	lea err4out, %rsi		# string to dest index
	mov $40, %rdx			# lenght of message 
	syscall     
	mov $60, %rax			# sys_exit
	mov $4, %rdi 			# array out of bounds
	syscall

# function for checking array index
# %rdi start address
# %rsi index
.type array_index @function
array_index:    
    # Check address
    cmp $0, %rdi
    je address_null_err

    # check index
    cmp $0, %rsi
    jl array_index_err
    
    cmp 8(%rdi), %rsi
    jge array_index_err
     
    ret

array_index_err:
    mov $1, %rax 			# sys_write
	mov $1, %rdi			# fd stdout 
	lea err2out, %rsi		# string to dest index
	mov $20, %rdx			# lenght of message 
	syscall 
	mov $60, %rax			# sys_exit
	mov $2, %rdi 			# array out of bounds
	syscall

address_null_err:
    mov $1, %rax 			# sys_write
	mov $1, %rdi			# fd stdout 
	lea err5out, %rsi		# string to dest index
	mov $20, %rdx			# lenght of message 
	syscall 
	mov $60, %rax			# sys_exit
	mov $5, %rdi 			# null pointer used
	syscall
    
divide_by_zero_err:
    mov $1, %rax 			# sys_write
	mov $1, %rdi			# fd stdout 
	lea err3out, %rsi		# string to dest index
	mov $15, %rdx			# lenght of message 
	syscall 
	mov $60, %rax			# sys_exit
	mov $3, %rdi 			# null pointer used
	syscall

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
    jg get_static_loop          # Jump if %rdi > 0

    ret

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

.section .data
    string_start: .quad 0
    heap_start: .quad 0    
	 heap_currpos: .quad 0
    lowspace: .quad 0
    highspace: .quad 0
    # Limit of 500 MB
    data_limit: .quad 0x20000000
	 # Initial size of 4096 bytes, 
    # the usual size of a virtual memory page
    data_size: .quad 0x1000

    err1out:
    	.ascii "meminit: error allocating memory\n" # length 33
    err2out:
        .ascii "array out of bounds\n"  # lenght 20
    err3out:
        .ascii "divide by zero\n"       # lenght 15
    err4out:
        .ascii "non-positive length for allocating array\n" # length 40
    err5out:
        .ascii "use of null pointer\n"  # lenght 20
    err6out:
    	.ascii "memory out of bounds\n" # lenght 21
    errlimit:
    	.ascii "1 GB limit exceeded\n"  # lenght 20

.section .bbs
    .equ buffer_size, 30
    .lcomm buffer, buffer_size
    .equ r_header, 16
    .equ a_header, 16
