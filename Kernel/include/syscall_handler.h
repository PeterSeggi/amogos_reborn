#include <stdint.h>

void syscall_handler();
void sys_write(uint64_t fd, uint64_t message, uint64_t length);
int sys_read(uint64_t fd, uint64_t buffer, uint64_t length);

void sys_draw(uint64_t bitmap, uint64_t hexColor, uint64_t height, uint64_t init_x, uint64_t init_y);
void sys_screenData(uint64_t screenHeight, uint64_t screenWidth, uint64_t fontSize, uint64_t drawSize);

int read_chars(int fd, char* buffer, int length);
void getScreenData(uint16_t * screenHeight, uint16_t * screenWidth, uint8_t * fontSize, uint8_t * drawSize);
