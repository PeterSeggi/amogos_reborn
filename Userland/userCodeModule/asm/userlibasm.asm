GLOBAL int_test
GLOBAL _print

section .text

;screen functions

;================================================================================================================================
;_print imprime en pantalla un string null terminated
;int 80h para usar la syscall
;IN: rdi=puntero a str; rsi=strlen; rdx: fd-> 1 STDOUT -> 2 STDERROR
;================================================================================================================================
;================================================================================================================================
_print:
    push rbp
	mov rbp, rsp

    mov rax, 1
    int 80h;

	mov rsp, rbp
	pop rbp
	ret

_read:
    push rbp
	mov rbp, rsp

    mov rax, 0
    int 80h;

	mov rsp, rbp
	pop rbp
	ret
    


int_test:
    push rbp
    mov rbp, rsp

    mov rdi, 1
    mov rsi, 2
    mov rdx, 3
    mov rax, 4

    int 80h

    mov rsp, rbp
    pop rbp
    ret

;================================================================================================================================


section .bss
    newLineString db "hello" 
    
