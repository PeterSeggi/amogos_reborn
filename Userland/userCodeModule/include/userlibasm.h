#ifndef _OURASMLIB_H_
#define _OURASMLIB_H_

void _print(int fd, char * string, int length);
int _read(int fd, char * buffer, int length);
int int_test();
void _draw(uint16_t * bitmap, uint32_t color, uint16_t height, uint64_t x, uint64_t y);
#endif
