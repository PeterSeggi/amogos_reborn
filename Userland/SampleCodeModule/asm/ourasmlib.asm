section .text

;screen functions
GLOBAL _print

;================================================================================================================================
;_print imprime en pantalla un string null terminated
;int 80h para usar la syscall
;IN: rdi=puntero a str; rsi=strlen; rdx: fd-> 1 STDOUT -> 2 STDERROR
;================================================================================================================================
;================================================================================================================================
_print:
    push rbp
	mov rbp, rsp
    push rbx
    push rcx
    push rdx
	pushf

    mov rbx, rdx   ;fd 
    mov rcx, rdi   ;paso el pointer;
    mov rdx, rsi   ;paso la cantidad de chars a imprimir
    mov rax, 0x04   ;sysID de sysWrite

    int 80h;

.end:
	popf
    pop rdx
    pop rcx
    pop rbx
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================