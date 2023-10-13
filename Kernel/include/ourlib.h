#ifndef OURLIB_H
#define OURLIB_H

#include <stdint.h>

//textmode write functions
void ourPrintColor(const char * string, char color);
void ourPrintCharColor(char character, char color);

//textmode time functions
void printDateTime();

#endif