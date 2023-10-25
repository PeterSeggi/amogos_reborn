GLOBAL int_test
GLOBAL _print
GLOBAL _read

GLOBAL _getDateTimeFormat
GLOBAL _getSeconds
GLOBAL _getMinutes
GLOBAL _getHours

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


;================================================================================================================================
;_getDateTimeFormat devuelve el formato del RTC
;en al retorna 0x00000xy0  donde y indica si es 24hs(1) o no(0) y x indica si es binario(1) o BCD(0)
;================================================================================================================================
;================================================================================================================================
_getDateTimeFormat:
	push rbp
	mov rbp, rsp
	pushf

	cli
	mov al, 0x0B	;leemos el registroB del RTC
	out 70h, al
	in al, 71h
	sti		;retomar interrupciones
	and al, 0x6	;retorno solo el bit 1 y 2

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;================================================================================================================================
;_getTimeElement funcion privada para devolver el elemento elegido del RTC (chequear formato con _getDateTimeFormat)
;recibe como parametro un numero entre 0-9 en rdi
;================================================================================================================================
;================================================================================================================================
_getTimeElement:
	push rbp
	mov rbp, rsp
	pushf

	mov al, 0x0A

.wait:
	cli		;esperamos a que no este actualizandose
	out 70h, al	;leemos el status
	in al, 71h
	sti
	and al, 0x80	;me quedo solo con el bit de timeupdate
	cmp al, 1	;no esta actualizando, puedo seguir
	je .wait

.read:	
	cli
	mov rax, rdi	;le paso que elemento quiero
	out 70h, al
	in al, 71h
	sti		;retomar interrupciones

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;================================================================================================================================
;_getSeconds devuelve los segundos del RTC
;================================================================================================================================
;================================================================================================================================
_getSeconds:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x00	;pido los segundos del RTC 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;================================================================================================================================
;_getMinutes devuelve los minutos del RTC
;================================================================================================================================
;================================================================================================================================
_getMinutes:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x02	;pido los minutos del RTC 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;================================================================================================================================
;_getHours devuelve las horas del RTC
;================================================================================================================================
;================================================================================================================================
_getHours:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x04	;pido las horas del RTC 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret


section .bss
    newLineString db "hello" 
    
