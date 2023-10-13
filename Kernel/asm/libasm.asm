GLOBAL cpuVendor
GLOBAL rtcInfo

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

    cli             ; /* Disable interrupts*/
    mov al, 0       ; /* Move index address*/
    ; /* since the 0x80 bit of al is not set, NMI is active */
    out 0x70,al     ; /* Copy address to CMOS register*/
    ; /* some kind of real delay here is probably best */
    in al,0x71      ; /* Fetch 1 byte to al*/
    sti             ; /* Enable interrupts*/

    mov rsp, rbp
    pop rbp
    ret
