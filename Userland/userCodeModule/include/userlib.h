#ifndef _OURLIB_H_
#define _OURLIB_H_

#include <stdint.h>

//escritura
void print(char * string);
void printError(char * string);
void printBase(uint64_t value, uint32_t base);
void printDec(uint64_t value);

//dibujo
void draw(uint16_t * bitmap, uint32_t color, uint16_t height, uint64_t x, uint64_t y);

// lectura
int read(char* buffer, int length);

// general purpose
int strlen(char * string);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

#endif
