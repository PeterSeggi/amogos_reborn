GLOBAL sem_lock_post
GLOBAL sem_lock_wait

SECTION .text

;recieves lock via rdi
sem_lock_wait:
    mov al, 1
    xchg al, [rdi]
    cmp al, 0
    jnz sem_lock_wait   ;spinlock "legal"
    ret

sem_lock_post:
    mov BYTE [rdi], 0
    ret