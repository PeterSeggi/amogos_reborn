#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdint.h>

void Minesweeper();
void place_bomb();
uint8_t get_bombs(uint8_t column, uint8_t row);

#endif