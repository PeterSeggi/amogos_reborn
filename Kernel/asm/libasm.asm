GLOBAL cpuVendor
GLOBAL rtcFunc
GLOBAL getFormat

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

rtcFunc:
	push rbp
	mov rbp, rsp

	cli				;Disable interrupts
	mov rax, rdi 	;Load the argument from the registers
    out 0x70, al    ;Copy the argument to CMOS address
    in al, 0x71     ;Fetch 1 byte to al
    sti             ;Enable interrupts

    mov rsp, rbp
    pop rbp
    ret

getFormat:
	push rbp
	mov rbp, rsp

	cli				;Disable interrupts
	mov al, 0x0B 	;En esa pos se encuentra el formato
    out 0x70, al    ;Copy the argument to CMOS address
    in al, 0x71     ;Fetch 1 byte to al
    sti             ;Enable interrupts
	and al, 0x6	;retorno solo el bit 1 y 2

    mov rsp, rbp
    pop rbp
    ret

