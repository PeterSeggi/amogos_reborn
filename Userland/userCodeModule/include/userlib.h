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
void sleep(uint32_t cant, uint32_t unidad);
  
//clock
void getClock(int *hrs, int *min, int *seg);
void printClock();

//Registers
void getRax(int *rax);

#endif
