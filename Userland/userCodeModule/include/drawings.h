#ifndef DRAW_H
#define DRAW_H

#define BLACK
#define WHITELINE
#define DARKGRAY
#define TRAINCOLOR

#include <stdint.h>

//bitmaps

#define dibHeight 16
#define dibWidth 16

extern uint16_t test[][dibHeight];
extern uint16_t snakehead_left[][dibHeight];

//functions
void draw_snakehead_left(uint32_t * colors, uint64_t init_x, uint64_t init_y);

#endif
