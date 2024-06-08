GLOBAL _cli
GLOBAL _sti
GLOBAL _print
GLOBAL _read
GLOBAL _draw
GLOBAL _screenData
GLOBAL _sleep
GLOBAL _getClock
GLOBAL _getRegs
GLOBAL _my_malloc
GLOBAL _my_free
GLOBAL _getMemState

GLOBAL _get_processes
GLOBAL _create_process

GLOBAL _sem_open
GLOBAL _sem_close
GLOBAL _sem_up
GLOBAL _sem_down

GLOBAL _changeSize
GLOBAL _opError 
GLOBAL _divError
GLOBAL _beep

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
    int 80h

	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================
;_read lee de entrada standard
;int 80h para usar la syscall
;IN: rdi=puntero a str; rsi=strlen; rdx: fd-> 1 STDIN -> 3 STDKEYS -> 4 STDLAST
;================================================================================================================================
;================================================================================================================================
_read:
    push rbp
	mov rbp, rsp

    mov rax, 0  ;sys_read ID
    int 80h;

	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================
;_getClock 
;int 80h para usar la syscall
;IN: rdi=* hrs;rsi=* min;rdx=* seg;
;================================================================================================================================
;================================================================================================================================
_getClock:
	push rbp
	mov rbp, rsp

    mov rax, 0x4e   ;sys_getClock ID
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
;IN: rdi=amount; rsi=unit;
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
;_getRegs 
;int 80h para usar la syscall
;IN: long *;
;================================================================================================================================
;================================================================================================================================
_getRegs:
	push rbp
	mov rbp, rsp

    mov rax, 0x78   ;sys_getRegs ID
    int 80h;
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================
;_my_malloc 
;int 80h para usar la syscall
;IN: long *;
;================================================================================================================================
;================================================================================================================================
_my_malloc:
	push rbp
	mov rbp, rsp

    mov rax, 0x86   ;sys_getRegs ID
    int 80h;

	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================
;_my_free 
;int 80h para usar la syscall
;IN: long *;
;================================================================================================================================
;================================================================================================================================
_my_free:
	push rbp
	mov rbp, rsp

    mov rax, 0x87   ;sys_getRegs ID
    int 80h;

	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================
;_getMemState 
;int 80h para usar la syscall
;IN: long *;
;================================================================================================================================
;================================================================================================================================
_getMemState:
	push rbp
	mov rbp, rsp

    mov rax, 0x88   ;sys_getRegs ID
    int 80h;
	mov rsp, rbp
	pop rbp
	ret

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

;================================================================================================================================
;_changeSize cambia el tamaño de font o draw en kernel
;int 80h para usar la syscall
;IN: rdi=newSize; rsi=fd 1=> font 2=> draw
;================================================================================================================================
;================================================================================================================================
_changeSize:
    push rbp
    mov rbp, rsp

    mov rax, 0x93   ;sys_changeSize ID

    int 80h     ;syscall 

.end:
    mov rsp, rbp
    pop rbp
    ret
;================================================================================================================================

;================================================================================================================================
;_get_processes 
;int 80h para usar la syscall
;IN: Process **;
;RET: amount of processes
;================================================================================================================================
;================================================================================================================================
_get_processes:
	push rbp
	mov rbp, rsp

    mov rax, 0xA0   ;sys_get_processes ID
    int 80h;
	mov rsp, rbp
	pop rbp
	ret
;================================================================================================================================

;================================================================================================================================
;_sem_open 
;int 80h para usar la syscall
;IN: const char* name, uint16_t value;
;RET: sem_t *;
;================================================================================================================================
;================================================================================================================================
_sem_open:
	push rbp
	mov rbp, rsp

    mov rax, 0xB0   ;sys_sem_open ID
    int 80h;
	mov rsp, rbp
	pop rbp
	ret
;================================================================================================================================

;================================================================================================================================
;_sem_close 
;int 80h para usar la syscall
;IN: sem_t *sem;
;RET: int;
;================================================================================================================================
;================================================================================================================================
_sem_close:
	push rbp
	mov rbp, rsp

    mov rax, 0xB1   ;sys_sem_close ID
    int 80h;
	mov rsp, rbp
	pop rbp
	ret
;================================================================================================================================

;================================================================================================================================
;_sem_up 
;int 80h para usar la syscall
;IN: sem_t *sem;
;RET: int;
;================================================================================================================================
;================================================================================================================================
_sem_up:
	push rbp
	mov rbp, rsp

    mov rax, 0xB2   ;sys_sem_up ID
    int 80h;
	mov rsp, rbp
	pop rbp
	ret
;================================================================================================================================

;================================================================================================================================
;_sem_down 
;int 80h para usar la syscall
;IN: sem_t *sem;
;RET: int;
;================================================================================================================================
;================================================================================================================================
_sem_down:
	push rbp
	mov rbp, rsp

    mov rax, 0xB3   ;sys_sem_down ID
    int 80h;
	mov rsp, rbp
	pop rbp
	ret
;================================================================================================================================


;================================================================================================================================
;_create_process 
;int 80h para usar la syscall
;IN: void* function, int priority, boolean orphan;
;================================================================================================================================
;================================================================================================================================
_create_process:
	push rbp
	mov rbp, rsp

    mov rax, 0xA1   ;sys_create_process ID
    int 80h;
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================
;_beep hace ruido!
; param 1 es frecuencia
; param 2 es duracion en milisegundos
;================================================================================================================================

_beep:
    push rbp
    mov rbp, rsp

    mov rax, 0x84   ;sys_speak ID

    int 80h         ;syscall 

.end:
    mov rsp, rbp
    pop rbp
    ret
    

;================================================================================================================================



_divError
    int 0
    ret


;================================================================================================================================
;_opError 
;ocasiona una invalid opcode exception
;================================================================================================================================
;================================================================================================================================
_opError:
    mov cr6, rax
    ret         ; ironico que este ese ret pero bueno, nunca sobra 

_cli:
	cli
	ret


_sti:
	sti
	ret
section .bss
    newLineString db "hello" 