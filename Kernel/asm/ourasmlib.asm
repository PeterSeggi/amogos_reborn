section .text

GLOBAL _getSeconds
GLOBAL _getMinutes
GLOBAL _getHours
GLOBAL _getDayWeek
GLOBAL _getDayMonth
GLOBAL _getMonth
GLOBAL _getYear

GLOBAL _getKey

;======================================================================================
;_getSeconds devuelve los segundos del RTC en ax
;======================================================================================
;======================================================================================
_getSeconds:
	push rbp
	mov rbp, rsp
	pushf

	mov al, 0
	out 70h, al
	in ax, 71h
.end:
	popf
	mov rsp, rbp
	pop rbp
	ret

;======================================================================================
;_getMinutes devuelve los minutos del RTC en ax
;======================================================================================
;======================================================================================
_getMinutes:
	push rbp
	mov rbp, rsp
	pushf

	mov al, 0x2
	out 70h, al
	in ax, 71h

	popf
	mov rsp, rbp
	pop rbp
	ret

;======================================================================================
;_getHours devuelve las horas del RTC en ax
;======================================================================================
;======================================================================================
_getHours:
	push rbp
	mov rbp, rsp
	pushf

	mov al, 0x4
	out 70h, al
	in ax, 71h

	popf
	mov rsp, rbp
	pop rbp
	ret

;======================================================================================
;_getDayWeek devuelve el dia de la semana del RTC en ax
;======================================================================================
;======================================================================================
_getDayWeek:
	push rbp
	mov rbp, rsp
	pushf

	mov al, 0x6
	out 70h, al
	in ax, 71h

	popf
	mov rsp, rbp
	pop rbp
	ret

;======================================================================================
;_getDayMonth devuelve el dia del mes del RTC en ax
;======================================================================================
;======================================================================================
_getDayMonth:
	push rbp
	mov rbp, rsp
	pushf

	mov al, 0x7
	out 70h, al
	in ax, 71h

	popf
	mov rsp, rbp
	pop rbp
	ret

;======================================================================================
;_getMonth devuelve el mes del RTC en ax
;======================================================================================
;======================================================================================
_getMonth:
	push rbp
	mov rbp, rsp
	pushf

	mov al, 0x8
	out 70h, al
	in ax, 71h

	popf
	mov rsp, rbp
	pop rbp
	ret

;======================================================================================
;_getYear devuelve el año del RTC en ax
;======================================================================================
;======================================================================================
_getYear:
	push rbp
	mov rbp, rsp
	pushf

	mov al, 0x9
	out 70h, al
	in ax, 71h

	popf
	mov rsp, rbp
	pop rbp
	ret

;======================================================================================
;_getKey devuelve scancode de la tecla presionada luego de esperar con pooling
;======================================================================================
;======================================================================================
_getKey:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

.loop:
	in al, 64h
	and al, 0x1	;esperar que el StatusReg este set
	cmp al, 0x1
	jne .loop

	in al, 60h	;guardo el scancode

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret