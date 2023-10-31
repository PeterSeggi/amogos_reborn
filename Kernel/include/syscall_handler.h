#include <stdint.h>

void syscall_handler();
void sys_write(uint64_t fd, uint64_t message, uint64_t length);
int sys_read(uint64_t fd, uint64_t buffer, uint64_t length);

int read_chars(char* buffer, int length);

void sys_sleep(uint32_t cant, uint32_t unidad);

void sys_gettimeofday(int *hrs, int *min, int *seg);

void sys_getRax(int *rax);
struct registers
{
	uint64_t rax, rbx, rcx, rdx, rbp, rsp, rsi,
	rdi, rip, r8, r9, r10, r11, r12, r13, r14, r15;
};

void sys_getRegisters(struct registers *regsStruct);
