#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void update_clock();
void set_clock_location(uint8_t *location);
void formatTime(uint8_t *sec, uint8_t *min, uint8_t *hour);
void formatDate(uint8_t *dayWeek, uint8_t *dayMonth, uint8_t *month, uint16_t *year);
uint8_t calculateMonthLastDay(uint8_t month, uint16_t year);

void printTime();
void printDate();

void sleep(int sec);
void tickscounter(int tickss);
void my_ints();


#endif
