#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>

#define BOARDCOLOR1 0x00B77460
#define BOARDCOLOR2 0x00F2997F

enum Direction {NONE=(0x00), UP=(0x80), DOWN=(0x40), RIGHT=(0x10), LEFT=(0x20)};

//main function
void Snake();

//setup
uint8_t initGame();
void tablero();
void snakeSetup(uint8_t snake, uint8_t * snakes);

//logic
void addApple(uint8_t row, uint8_t column);
void addSnake(uint8_t row, uint8_t column, uint8_t elem, enum Direction dir);
void putSnake(uint8_t row, uint8_t column, uint8_t snake);

//directions
void mov_snake_down(uint8_t snake);
void mov_snake_up(uint8_t snake);
void mov_snake_left(uint8_t snake);
void mov_snake_right(uint8_t snake);

void checkWithOthersDir( enum Direction currenDir, uint8_t currentRow, uint8_t currentCol, uint8_t othersInfo);
enum Direction getDirection(uint8_t row, uint8_t column);

void saveHeadPosition(uint8_t snake, uint8_t column, uint8_t row);

//checks
uint8_t checkUp(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkDown(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkLeft(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkRight(uint8_t row, uint8_t column, uint8_t value);



#endif