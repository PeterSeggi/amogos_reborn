#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void update_clock();
void set_clock_location(uint8_t *location);
void sleep(int sec);

#endif
