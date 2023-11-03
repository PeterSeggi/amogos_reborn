#include <stdint.h>

void syscall_handler();
void sys_write(uint64_t fd, uint64_t message, uint64_t length);
int sys_read(uint64_t fd, uint64_t buffer, uint64_t length);

int read_chars(int fd, char* buffer, int length);

void sys_sleep(uint32_t cant, uint32_t unidad);

void sys_gettimeofday(int *hrs, int *min, int *seg);

int sys_registers(long regs[]);
