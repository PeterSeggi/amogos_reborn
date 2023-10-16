section .text

;screen functions
GLOBAL _print

;================================================================================================================================
;_print imprime en pantalla un string null terminated
;int 80h para usar la syscall
;IN: rdi=puntero a str; rsi=strlen
;================================================================================================================================
;================================================================================================================================
_print:
    push rbp
	mov rbp, rsp
    push rbx
    push rcx
    push rdx
	pushf

    mov rcx, rdi   ;paso el pointer;
    mov rdx, rsi   ;paso la cantidad de chars a imprimir
    mov rax, 0x04   ;sysID de sysWrite
    mov rbx, 0x01   ;fd STDOUT

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