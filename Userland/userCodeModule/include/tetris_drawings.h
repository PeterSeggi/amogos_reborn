#ifndef TETRIS_DRAW_H
#define TETRIS_DRAW_H

#include <stdint.h>
//================================================================================================================================
//bitmaps
#define squareHeight 16
#define squareWidth 16

//================================================================================================================================
//functions
void draw_tetris_board();
void set_init_values(uint16_t screenHeight, uint16_t screenWidth, uint8_t squareSpaceHeight, uint8_t squareSpaceWidth);

#endif