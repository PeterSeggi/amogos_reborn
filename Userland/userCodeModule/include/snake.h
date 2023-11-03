#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>

#define BOARDCOLOR1 0x00B77460
#define BOARDCOLOR2 0x00F2997F

#define PLAYER1_DEFAULT_COLOR 0x00FAE425
#define PLAYER2_DEFAULT_COLOR 0x00FF71BE

enum Direction {NONE=(0x00), UP=(0x80), DOWN=(0x40), RIGHT=(0x10), LEFT=(0x20)};

//main function
void Snake(uint8_t players, uint32_t color1, uint32_t color2);

//setup
uint8_t initGame();
void tablero();
void snakeSetup(uint8_t snake);

//logic
void addApple();
void addSnake(uint8_t row, uint8_t column, uint8_t elem, enum Direction dir);
void putSnake(uint8_t row, uint8_t column, uint8_t snake);

void printPoints(uint8_t snake);

//directions
uint8_t slither(enum Direction dir, uint8_t snake);

enum Direction getDirection(int row, int column);

uint8_t changePosition(uint8_t column, uint8_t row, enum Direction dir, uint8_t snake);
void saveTailPosition(uint8_t snake, uint8_t column, uint8_t row);
void saveHeadPosition(uint8_t snake, uint8_t column, uint8_t row);

//getters
uint8_t getSnakeTailCol(uint8_t snake);
uint8_t getSnakeTailRow(uint8_t snake);
uint8_t getSnakeHeadCol(uint8_t snake);
uint8_t getSnakeHeadRow(uint8_t snake);
uint32_t getSnakeColor(uint8_t snake);

//checks
uint8_t checkUp(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkDown(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkLeft(uint8_t row, uint8_t column, uint8_t value);
uint8_t checkRight(uint8_t row, uint8_t column, uint8_t value);



#endif