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
void change_font(int size);

//wait
void miliSleep(uint64_t milis);
void sleep_once();

//dibujo
void draw(uint16_t * bitmap, uint32_t color, uint16_t height, uint64_t x, uint64_t y);
void getScreenData(uint16_t * screenHeight, uint16_t * screenWidth, uint8_t * fontSize, uint8_t * drawSize);
int getFontSize();

// lectura
int read(char* buffer, int length);
int readRaw(char* buffer, int length);
int readLast(char* buffer, int length);

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

#endif
