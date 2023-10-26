#include <stdint.h>

void syscall_handler();
void sys_write(uint64_t fd, uint64_t message, uint64_t length);
int sys_read(uint64_t fd, uint64_t buffer, uint64_t length);

void sys_draw(uint64_t bitmap, uint64_t hexColor, uint64_t height, uint16_t init_x, uint64_t init_y);

int read_chars(char* buffer, int length);
