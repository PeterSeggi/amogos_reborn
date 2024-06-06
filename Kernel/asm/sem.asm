GLOBAL sem_lock_post
GLOBAL sem_lock_wait

;recieves lock via rdi
sem_lock_wait:
    mov rax, 1
    xchg rax, BYTE [rdi]
    cmp rax, 0
    jnz sem_lock_wait   ;spinlock "legal"
    ret

sem_lock_post:
    mov lock, 0
    ret