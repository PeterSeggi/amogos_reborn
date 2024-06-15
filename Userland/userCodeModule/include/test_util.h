#ifndef RAND_H_
#define RAND_H_

#include <stdint.h>

//test_util
uint16_t GetUint();
uint16_t GetUniform(uint16_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
void bussy_wait(uint64_t n);
void endless_loop();
void endless_loop_print(uint64_t wait);

//tests
uint64_t test_mm(uint64_t argc, char *argv[]);

#endif