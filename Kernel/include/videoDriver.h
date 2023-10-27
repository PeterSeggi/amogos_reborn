#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void draw_rectangle(uint64_t ancho, uint64_t alto, uint32_t color, uint64_t init_x, uint64_t init_y);


void putChar(uint8_t character, uint32_t colorFont, uint32_t colorBg, uint64_t init_x, uint64_t init_y);
void printChar(uint8_t chararacter);
void printCharColor(uint8_t character, uint32_t fontColor, uint32_t bgColor);


void print(const uint8_t * string);
void printCant(const uint8_t * string, uint64_t cant);
void printColor(const uint8_t * string, uint32_t fontColor, uint32_t bgColor);
void printColorCant(const uint8_t * string, uint64_t cant, uint32_t fontColor, uint32_t bgColor);

void newLine();
void delChar();
void clear();
void repoCursor();

void printBase(uint64_t value, uint32_t base);
void printDec(uint64_t value);
void printHex(uint64_t value);

#endif
