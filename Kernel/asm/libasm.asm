GLOBAL cpuVendor
GLOBAL rtcInfo

;time functions
GLOBAL _getDateTimeFormat
GLOBAL _getSeconds
GLOBAL _getMinutes
GLOBAL _getHours
GLOBAL _getDayWeek
GLOBAL _getDayMonth
GLOBAL _getMonth
GLOBAL _getYear

;keyboard functions
GLOBAL _getKey
GLOBAL _getKeyPool

;syscall functions
GLOBAL _getSysID
GLOBAL _getBReg
GLOBAL _getCReg
GLOBAL _getDReg

GLOBAL _saveRegs

;in/out functions
GLOBAL _outb
GLOBAL _inb

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

rtcInfo:
    push rbp
    mov rbp, rsp

    cli             ;  Disable interrupts
    mov al, dil     ;  Move index address
                    ;  since the 0x80 bit of al is not set, NMI is active 
    out 0x70,al     ;  Copy address to CMOS register
                    ;  some kind of real delay here is probably best 
    in al,0x71      ;  Fetch 1 byte to al
    sti             ;  Enable interrupts

    mov rsp, rbp
    pop rbp
    ret


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

;================================================================================================================================

;================================================================================================================================
;_getDayWeek devuelve el día de la semana del RTC (1: domingo, 7: sabado)
;================================================================================================================================
;================================================================================================================================
_getDayWeek:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x06	;pido dia de la semana del RTC 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;================================================================================================================================
;_getDayMonth devuelve el día del mes del RTC
;================================================================================================================================
;================================================================================================================================
_getDayMonth:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x07	;pido dia del mes del RTC 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;================================================================================================================================
;_getMonth devuelve el mes del RTC
;================================================================================================================================
;================================================================================================================================
_getMonth:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x08	;pido mes del RTC 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;================================================================================================================================
;_getYear devuelve el año del RTC
;================================================================================================================================
;================================================================================================================================
_getYear:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x09	;pido año del RTC 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;================================================================================================================================
;_getKeyPool devuelve scancode de la tecla presionada luego de esperar con pooling
;================================================================================================================================
;================================================================================================================================
_getKeyPool:
	push rbp
	mov rbp, rsp
	pushf

.loop:
	in al, 64h
	and al, 0x1	;esperar que el StatusReg este set
	cmp al, 0x1
	jne .loop

	in al, 60h	;guardo el scancode

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret
	
;================================================================================================================================

;================================================================================================================================
;_getKey devuelve scancode de la tecla presionada
;================================================================================================================================
;================================================================================================================================
_getKey:
	push rbp
	mov rbp, rsp
	pushf

	in al, 60h	;guardo el scancode

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret
	
;================================================================================================================================

;================================================================================================================================
;_getSysID retorna el ID. (como rax suele pisarse mucho, si fue int80h quedo en r12)
;================================================================================================================================
;================================================================================================================================
_getSysID:
	push rbp
	mov rbp, rsp
	pushf

	mov rax, r12

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret	

;================================================================================================================================

;================================================================================================================================
;_getBReg devuelve el registro
;================================================================================================================================
;================================================================================================================================
_getBReg:
	push rbp
	mov rbp, rsp
	pushf

	mov rax, rbx	;devolvemos rbx

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;================================================================================================================================
;_getCReg devuelve el registro
;================================================================================================================================
;================================================================================================================================
_getCReg:
	push rbp
	mov rbp, rsp
	pushf

	mov rax, rcx	;devolvemos rcx

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

;================================================================================================================================
;_getDReg devuelve el registro
;================================================================================================================================
;================================================================================================================================
_getDReg:
	push rbp
	mov rbp, rsp
	pushf

	mov rax, rdx	;devolvemos rdx

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

_saveRegs:
	push rbp
	mov rbp, rsp

	int 77h

	mov rsp, rbp
	pop rbp
	ret


;================================================================================================================================
;_outb hace out a param 1 de lo que hay en param 2 
;_inb  hace in de un byte de param 1
;================================================================================================================================
;================================================================================================================================
_outb:
	mov eax, esi
	mov edx, edi
	out dx, al
	ret

_inb:
	mov edx, edi
	in	al, dx
	ret

;================================================================================================================================
