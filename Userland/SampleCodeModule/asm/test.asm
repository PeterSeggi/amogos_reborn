GLOBAL int_test

section .text

int_test:
    push rbp
    mov rbp, rsp

    int 80h

    mov rax, 0x48

    mov rsp, rbp
    pop rbp
    ret

    
