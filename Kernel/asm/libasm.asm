GLOBAL cpuVendor
GLOBAL rtcFunc
GLOBAL getFormat
GLOBAL getKey

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

getKey:
	push rbp
	mov rbp, rsp

	;primero nos aseguramos q se pueda leer 
.loop:
	in al, 64h  ;guardamos el bit 0
	and al, 1   ;pedazo ilegal de knd
	cmp al, 1 	 ;ya q es el q debe estar en 1 para q se pueda leer el port
	jne .loop	 ;si no estaba en 1 vuelvo al loop

	;ya se puede leer
	in al, 60h ;guardo la info

	mov rsp, rbp
	pop rbp
	ret
