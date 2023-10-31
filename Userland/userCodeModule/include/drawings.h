#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

//colors

#define LIGHTGRAY 0x00B8B8B8
#define WHITELINE 0x00FBFBFB
#define WHITE 0x00F6F6F6
#define BLACK 0x00000000
#define DARKGRAY 0x00414141
#define DARKERGRAY 0x00343434
#define RED 0x00FF290E

//bitmaps
#define dibHeight 16
#define dibWidth 16

extern uint16_t square[][dibHeight];

extern uint16_t apple[][dibHeight];

extern uint16_t snakehead_left[][dibHeight];
extern uint16_t snakehead_right[][dibHeight];
extern uint16_t snakehead_up[][dibHeight];
extern uint16_t snakehead_down[][dibHeight];

extern uint16_t snakecurve_downright[][dibHeight];
extern uint16_t snakecurve_downleft[][dibHeight];
extern uint16_t snakecurve_upright[][dibHeight];
extern uint16_t snakecurve_upleft[][dibHeight];

extern uint16_t snakebody_horizontal1[][dibHeight];
extern uint16_t snakebody_horizontal2[][dibHeight];
extern uint16_t snakebody_vertical[][dibHeight];

//functions
void draw_snakehead_left(uint64_t init_x, uint64_t init_y, uint32_t traincolor);
void draw_snakehead_right(uint64_t init_x, uint64_t init_y, uint32_t traincolor);
void draw_snakehead_up(uint64_t init_x, uint64_t init_y, uint32_t traincolor);
void draw_snakehead_down(uint64_t init_x, uint64_t init_y, uint32_t traincolor);

void draw_snakecurve_downright(uint64_t init_x, uint64_t init_y, uint32_t traincolor);
void draw_snakecurve_downleft(uint64_t init_x, uint64_t init_y, uint32_t traincolor);
void draw_snakecurve_upright(uint64_t init_x, uint64_t init_y, uint32_t traincolor);
void draw_snakecurve_upleft(uint64_t init_x, uint64_t init_y, uint32_t traincolor);

void draw_snakebody_horizontal1(uint64_t init_x, uint64_t init_y, uint32_t traincolor);
void draw_snakebody_horizontal2(uint64_t init_x, uint64_t init_y, uint32_t traincolor);
void draw_snakebody_vertical(uint64_t init_x, uint64_t init_y, uint32_t traincolor);

void draw_snake(uint16_t bitmap[][dibHeight], uint16_t layers, uint32_t colors[], uint64_t init_x, uint64_t init_y, uint32_t traincolor);

void draw_manzana(uint64_t init_x, uint64_t init_y);

uint8_t checkUp(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkDown(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkLeft(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkRight(uint8_t row, uint8_t column, uint8_t value);

#endif
