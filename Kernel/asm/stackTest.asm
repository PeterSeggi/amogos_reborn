

GLOBAL createStack
GLOBAL stackTest
GLOBAL stackPrep
GLOBAL stackUnprep
section .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

createStack:
	push 0x0 	;SS=0x0
	;mov rsp, rdi;RSP=argumento que le paso a la funcion con el stack 
	push 0x202	;RFLAGS=0x202
	push 0x8	;CS=0x8 al comienzo del proceso
	pushState	;gpr
    ret

stackPrep:
	push rsp
	pushf
	get_rip
	push rax	;el rip antes de llamar a esta linea
	pushState
	ret
stackUnprep:
	popState	;gpr
	add rsp, 0x8	
	;pop rip		;RIP
	popf		;rflags
	pop rsp		;RSP
	
	sub rsp, ;la indicada
    ret

stackTest:
	pushState
    mov rsp, rdi
    push 1
    push 2
    push 3
    push 10
    pop rax
    pop rax
    pop rax
    pop rax
    ret

get_rip:
    pop rax             ; Pop the address into rax (now rax contains the value of RIP)
    
    ; Now rax contains the value of the instruction pointer (RIP)
    ; You can push rax onto the stack if you need to store it

    push rax            ; Push the value of RIP onto the stack
	ret