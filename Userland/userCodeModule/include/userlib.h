#ifndef _OURLIB_H_
#define _OURLIB_H_

#include <stdint.h>

//escritura
void print(char * string);
void printChar(char charToPrint);
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
int strcmp(const char *str1, const char *str2);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

#endif
