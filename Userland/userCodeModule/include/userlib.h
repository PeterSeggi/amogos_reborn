#ifndef _OURLIB_H_
#define _OURLIB_H_

#include <stdint.h>

//escritura
void print(char * string);
void printError(char * string);
void printBase(uint64_t value, uint32_t base);
void printDec(uint64_t value);

// lectura
int read(char* buffer, int length);

// general purpose
int strlen(char * string);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

//sleep
void nanosleep(int cant, int unidad);

//clock
void formatTime(uint8_t *sec, uint8_t *min, uint8_t *hour);
void printTime();
void sleep(int sec);
void timer_handler();
int ticks_elapsed();

#endif
