#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void draw_rectangle(uint64_t ancho, uint64_t alto, uint32_t color, uint64_t init_x, uint64_t init_y);
void putChar(uint8_t character, uint32_t colorFont, uint32_t colorBg, uint64_t init_x, uint64_t init_y);

uint16_t getHeight();

#endif
