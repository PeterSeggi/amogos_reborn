#ifndef _OURASMLIB_H_
#define _OURASMLIB_H_

void _print(int fd, char * string, int length);
int _read(int fd, char * buffer, int length);
int int_test();
void _sleep(int cant, int unidad);
void _getClock(int *hrs, int *min, int *seg);
void _getRax(int *rax);
int _getRbx();
struct registers
{
	int rax, rbx, rcx, rdx, rbp, rsp, rsi,
	rdi, r8, r9, r10, r11, r12, r13, r14, r15;
};
void _getRegisters(struct registers *regsStruct);

#endif
