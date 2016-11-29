.code64
.section .text
.global start_thread
.global switch_threads

start_thread:
	movq %rax, %rdi

	callq free_thread

	popq %rax 
	popq %rdi 

	callq *%rax

	callq finish_thread
	loop: jmp loop

switch_threads:

	pushq %rbp    
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	callq get_current_thread

	movq %rsp, (%rdi)
	movq %rsi, %rsp  

	popq %r15  
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp

	ret
