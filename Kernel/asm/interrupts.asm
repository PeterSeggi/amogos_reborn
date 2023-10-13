;section .text

;_irq00Handler:
;	irqHandlerMaster 0

;%macro irqHandlerMaster 1
;	pushState
;	mov rdi, %1; pasaje de parametro
;	call irqDispatcher
;
;	;signal pic EOI(end of interrupt)
;	mov al, 20h
;	out 20h, al

;	popState
;	iretq
;%endmacro