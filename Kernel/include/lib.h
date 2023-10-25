#ifndef LIB_H
#define LIB_H

#include <cstdint>
#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

// asm functions
char *cpuVendor(char *result);
int rtcInfo(int value);
uint32_t readKey();

// base changing
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);


#endif
