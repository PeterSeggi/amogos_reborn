#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

int pow(int base, int exp);

void k_strcpy(char *destination, const char *source);
int k_strlen(const char * string);
int k_strcmp(const char *str1, const char *str2);
char *k_strdup(const char *s);

/*----------------------
  | ASM functions
  -----------------------*/
char *cpuVendor(char *result);
int rtcInfo(int value);
long* _regsInterrupt();

/**
*@brief     Initializes the stack for a new process.
*@param[in] rsp New process RSP.
*@param[in] rip New process RIP.
*@param[in] argc New process argc.
*@param[in] argv New process argv.
*@return    New process RIP.
*/
uint64_t _initialize_stack(void * rsp, void * rip, int argc, char ** argv);

/**
*@brief     Returns RTC seconds.
*@return    RTC seconds.
*/
int _getSeconds();

/**
*@brief     Returns RTC minutes.
*@return    RTC minutes.
*/
int _getMinutes();

/**
*@brief     Returns RTC hours.
*@return    RTC hours.
*/
int _getHours();

/**
*@brief     Returns RTC minutes.
*@return    RTC minutes.
*/
int _getDay();

/**
*@brief     Returns RTC month.
*@return    RTC month.
*/
int _getMonth();

/**
*@brief     Returns RTC year.
*@return    RTC year.
*/
int _getYear();

/**
*@brief     Returns RTC day of the week.
*@return    RTC day of the week.
*@note      SUN=1, SAT=7.
*/
int _getDayWeek();

/**
*@brief     Returns RTC day of the month.
*@return    RTC day of the month.
*/
int _getDayMonth();

/**
*@brief     Returns RTC format.
*@return    0x00000rt0. Where t=1 for 24hs, 0 otherwise. r=1 for binary, 0 for BCD.
*/
int _getDateTimeFormat();

/**
*@brief     Reads pressed key scancode.
*@return    Pressed key scancode.
*@note      To be used in a keyboard handler after interrupt.
*/
int _getKey();

/**
*@brief     Forces an int20h, so it will force to schedule.
*@note      Does a "sti" before the interrupt.
*/
void _force_schedule();

/**
*@brief     out reg, value.
*@param[in] reg Where to make the out.
*@param[in] value value.
*/
void _outb(uint32_t reg, uint32_t value);

/**
*@brief     in of one byte.
*@param[in] reg Where to make the in.
*@return
*/
uint8_t _inb(uint32_t reg);

#endif
