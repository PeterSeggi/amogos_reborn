#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string);
void ncPrintColor(const char * string, int color);
void ncPrintChar(char character);
void ncPrintCharColor(char character, int color);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
uint8_t * getCurrentVideo();
void setCurrentVideo(uint8_t *location);

#endif
