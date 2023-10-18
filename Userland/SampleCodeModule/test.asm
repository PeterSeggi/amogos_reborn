GLOBAL test_int

section .text:

test_int:
    push rbp
    mov rbp, rsp

    int 80h

    mov rsp, rbp
    pop rbp
    ret


