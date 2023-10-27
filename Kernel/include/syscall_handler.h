#include <stdint.h>

void syscall_handler();
void sys_write(uint64_t fd, uint64_t message, uint64_t length);
int sys_read(uint64_t fd, uint64_t buffer, uint64_t length);

int read_chars(int fd, char* buffer, int length);
