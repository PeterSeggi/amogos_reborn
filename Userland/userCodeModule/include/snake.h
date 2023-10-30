#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>

#define BOARDCOLOR1 0x00B77460
#define BOARDCOLOR2 0x00F2997F

void Snake();

uint8_t initGame();
void tablero();

void addApple(uint8_t row, uint8_t column);
void addSnake(uint8_t row, uint8_t column, uint8_t elem);
void putSnake(uint8_t row, uint8_t column, uint8_t snake);

uint8_t checkUp(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkDown(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkLeft(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkRight(uint8_t row, uint8_t column, uint8_t value);



#endif