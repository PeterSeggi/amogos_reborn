GLOBAL int_test
GLOBAL _print
GLOBAL _read
GLOBAL _draw
GLOBAL _screenData
GLOBAL _sleep
GLOBAL _halt
GLOBAL _sleep
GLOBAL _getClock

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
    
_halt:
    hlt
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

_sleep:
	push rbp
	mov rbp, rsp

    mov rax, 0x23
    int 80h;

	mov rsp, rbp
	pop rbp
	ret

_getClock:
	push rbp
	mov rbp, rsp

    mov rax, 0x4e
    int 80h;

	mov rsp, rbp
	pop rbp
	ret
;================================================================================================================================
;_draw dibuja en pantalla un bitmap (uint16_t width)
;int 80h para usar la syscall
;IN: rdi=bitmap; rsi=color; rdx: bitmap height; rcx: init_x position; r8: init_y position
;================================================================================================================================
;================================================================================================================================
_draw:
    push rbp
    mov rbp, rsp

    mov rax, 0x77   ;sys_draw ID

    int 80h     ;syscall 

.end:
    mov rsp, rbp
    pop rbp
    ret
;================================================================================================================================

;================================================================================================================================
;_sleep duerme
;int 80h para usar la syscall
;IN: rdi=amount
;================================================================================================================================
;================================================================================================================================
_sleep:
    push rbp
    mov rbp, rsp

    mov rax, 0x23   ;sys_sleep ID

    int 80h     ;syscall 

.end:
    mov rsp, rbp
    pop rbp
    ret
;================================================================================================================================


;================================================================================================================================
;_screenData devuelve informacion del videoDriver
;int 80h para usar la syscall
;IN: rdi=screenHeight; rsi=screenWidth; rdx:fontSize; rcx: drawSize;
;================================================================================================================================
;================================================================================================================================
_screenData:
    push rbp
    mov rbp, rsp

    mov rax, 0x83   ;sys_screenData ID

    int 80h     ;syscall 

.end:
    mov rsp, rbp
    pop rbp
    ret
;================================================================================================================================

section .bss
    newLineString db "hello" 
    
