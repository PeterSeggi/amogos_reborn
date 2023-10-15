#ifndef OURLIB_H
#define OURLIB_H

#include <stdint.h>

//textmode write functions
void ourPrintColor(const char * string, char color);
void ourPrintCharColor(char character, char color);

//textmode time functions
void printDateTime();
void formatTime(uint8_t* sec, uint8_t* min, uint8_t* hour);
void formatDate(uint8_t* dayWeek, uint8_t* dayMonth, uint8_t* month, uint16_t* year);
void printClock();
void printDateTime();
void printDate();
void printTime();

#endif