
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq77Handler
GLOBAL _irq128Handler

GLOBAL _exception0Handler
GLOBAL _regsInterrupt

EXTERN irqDispatcher
EXTERN syscall_handler
EXTERN exceptionDispatcher
EXTERN getStackBase

SECTION .text

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

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di
    out	    21h,al
    pop     rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	    0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Registers
_irq77Handler:
	irqHandlerMaster 0x77

;syscall
_irq128Handler:
    mov rcx, rax
    call syscall_handler
	; irqHandlerMaster 128



;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

haltcpu:
	cli
	hlt
	ret

	
_regsInterrupt:
	mov [regsBuf], rax 	
	mov [regsBuf+8], rbx
	mov [regsBuf+8*2], rcx
	mov [regsBuf+8*3], rdx
	mov [regsBuf+8*4], rsi
	mov [regsBuf+8*5], rdi
	mov [regsBuf+8*6], rbp
	mov [regsBuf+8*7], rsp
	mov [regsBuf+8*8], r8
	mov [regsBuf+8*9], r9 
	mov [regsBuf+8*10], r10
	mov [regsBuf+8*11], r11
	mov [regsBuf+8*12], r12
	mov [regsBuf+8*13], r13
	mov [regsBuf+8*14], r14
	mov [regsBuf+8*15], r15

	push rbp
	mov rbp, rsp

	mov rax, [rsp]			; RIP
	mov [regsBuf+8*16], rax			
	mov rax, [rsp+8*2]		; RFLAGS
	mov [regsBuf+8*17], rax
	mov rax, regsBuf

	mov rsp, rbp
	pop rbp
	ret 
	    
    ;mov [rsp], userland_direc
    ;call getStackBase
    ;mov [rsp+24], rax   	;acomodo el valor de retorno
	;iretq

SECTION .bss
	aux resq 1
	regsBuf resq 18

section .rodata
	userland_direc equ 0x400000
