#ifndef _OURLIB_H_
#define _OURLIB_H_

#include <stdint.h>

//escritura
void print(char * string);
void printChar(char charToPrint);
void printCant(char* string, int cant);
void printError(char * string);
void printBase(uint64_t value, uint32_t base);
void printDec(uint64_t value);
void printHex(uint64_t value);
void clearScreen();

// lectura
int read(char* buffer, int length);
int readRaw(char* buffer, int length);

// general purpose
int strlen(char * string);
void strcpy(char *destination, const char *source);
int strcmp(const char *str1, const char *str2);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
int mod(int val, int base);

//sleep
void sleep(uint32_t cant, uint32_t unidad);
  
//clock
void getClock(int *hrs, int *min, int *seg);

//Registers
void getRegs(char * regs);
#endif
