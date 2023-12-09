#ifndef MINE_DRAW_H
#define MINE_DRAW_H

#include <stdint.h>

//================================================================================================================================
//colors
#define LIGHTGRAY 0x00F8F8F8
#define GRAY 0x00E1E1E1
#define DARKGRAY 0x00919191
#define DARKERGRAY 0x008B8B8B
#define BLACK 0x00000000
#define RED 0x00F20000
#define YELLOW 0x00FFF500
#define WHITE 0x00FFFFFF

#define NUM1COLOR 0x00004BF2
#define NUM2COLOR 0x00009D2F
#define NUM3COLOR 0x00FF2000
#define NUM4COLOR 0x0002009C
#define NUM5COLOR 0x009C0700
#define NUM6COLOR 0x00339C9C
#define NUM7COLOR 0x00BE20A0
#define NUM8COLOR 0x006F6F6F

//================================================================================================================================
//bitmaps
#define boxHeight 16
#define boxWidth 16
#define bombHeight 32

extern uint16_t box[][boxHeight];

extern uint16_t pressed_box[][boxHeight];

extern uint16_t bomb[][bombHeight];

extern uint16_t flag[][boxHeight];

extern uint16_t numbers[][boxHeight];

//================================================================================================================================
//functions
void draw_box(uint64_t init_x, uint64_t init_y);
void draw_pressed_box(uint64_t init_x, uint64_t init_y);
void draw_flag(uint64_t init_x, uint64_t init_y);
void draw_bomb(uint64_t init_x, uint64_t init_y);
void draw_minebox(uint16_t bitmap[][boxHeight], uint16_t layers, uint32_t colors[], uint64_t init_x, uint64_t init_y);
void draw_proximity(uint64_t init_x, uint64_t init_y, uint8_t amount);

#endif