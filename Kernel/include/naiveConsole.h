#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

void ourPrintCharColor(char character, char color);
void ourPrintColor(const char * string, char color);
void ourPrintSpace(const char * string, uint8_t column, uint8_t row);
void ourPrintDecSpace(uint64_t value, uint8_t column, uint8_t row);
void ourPrintBaseSpace(uint64_t value, uint32_t base, uint8_t column, uint8_t row);

#endif