#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

//bitmaps

#define dibHeight 16
#define dibWidth 16

extern uint16_t square[][dibHeight];

extern uint16_t apple[][dibHeight];

extern uint16_t snakehead_left[][dibHeight];

//functions
void draw_snakehead_left(uint32_t * colors, uint64_t init_x, uint64_t init_y);
void draw_snake(uint16_t bitmap[][dibHeight], uint16_t layers, uint32_t colors[], uint64_t init_x, uint64_t init_y);

void draw_manzana(uint64_t init_x, uint64_t init_y);

#endif
