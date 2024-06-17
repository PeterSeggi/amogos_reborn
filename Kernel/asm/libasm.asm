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


;in/out functions
GLOBAL _outb
GLOBAL _inb

;process handling functions
GLOBAL _force_schedule
GLOBAL _initialize_stack

EXTERN popState
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

;================================================================================================================================

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

;================================================================================================================================

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

_initialize_stack:
	push rbp
	mov rbp, rsp
	
	mov rsp, rdi		;stands on the process stack
	push 0x0 ;SS
	push rdi ;RSP
	push 0x202 ;RFLAGS
	push 0x8   ;CS
	push rsi ;RIP
	mov rdi, rdx	;sets argc as the current first argument of the process
	mov rsi, rcx 	;sets argv as the current second argument of the process
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

	mov rax, rsp
	mov rsp, rbp
	pop rbp
	
	ret
	
;================================================================================================================================

_getDateTimeFormat:
	push rbp
	mov rbp, rsp
	pushf

	cli
	mov al, 0x0B	;Read RTC's B register
	out 70h, al
	in al, 71h
	sti		
	and al, 0x6	;returns only bit 2 and 1 (0x00000110 mask)

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

_getSeconds:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x00	;0x00 -> RTC seconds 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

_getMinutes:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x02	;0x02 -> RTC minutes 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

_getHours:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x04	;0x04 -> RTC hours 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

_getDayWeek:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x06	;0x06 -> RTC day of the week 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

_getDayMonth:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x07	;0x06 -> RTC day of the month 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

_getMonth:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x08	;0x08 -> RTC month 
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

_getYear:
	push rbp
	mov rbp, rsp
	push rdi
	pushf

	mov rdi, 0x09	;0x09 -> RTC year
	call _getTimeElement

.end:
	popf
	pop rdi
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================

_getKey:
	push rbp
	mov rbp, rsp
	pushf

	in al, 60h	;saves scancode

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret
	
;================================================================================================================================

_force_schedule:
	sti
	int 20h	;force a tick 
	ret

;================================================================================================================================
_outb:
	mov eax, esi
	mov edx, edi
	out dx, al
	ret

;================================================================================================================================

_inb:
	mov edx, edi
	in	al, dx
	ret

;================================================================================================================================


;/*----------------------
;  | Local functions
;  -----------------------*/

;================================================================================================================================
;_getTimeElement 
;IN: [rdi] number between 0-9 that identifies the element
;Notes: Remember to check format of the RTC
;================================================================================================================================
;================================================================================================================================
_getTimeElement:
	push rbp
	mov rbp, rsp
	pushf

	mov al, 0x0A

.wait:
	cli		
	out 70h, al		;read status
	in al, 71h
	sti
	and al, 0x80	;keeps only timeupdate bit
	cmp al, 1		;isn't updating, keeps going. Otherwise loops at wait
	je .wait

.read:	
	cli
	mov rax, rdi	;passes desired element
	out 70h, al
	in al, 71h
	sti		

.end:
	popf
	mov rsp, rbp
	pop rbp
	ret

;================================================================================================================================
