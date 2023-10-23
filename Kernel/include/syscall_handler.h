#include <stdint.h>

void syscall_handler();
void sys_write(uint64_t fd, uint64_t message, uint64_t length);
void sys_read(uint64_t fd, uint64_t buffer, uint64_t length);
