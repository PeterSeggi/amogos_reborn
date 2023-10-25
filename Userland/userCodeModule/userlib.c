#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/file_descriptors.h"

static char buffer[64] = {'0'};

//================================================================================================================================
// Writting
//================================================================================================================================
//================================================================================================================================

void print(char * string){
    _print(STDOUT, string, strlen(string));
}

void printError(char * string){
    _print(STDERROR, string, strlen(string));
}

int read(char* buffer, int length){
    return _read(STDIN, buffer, length);
}

int strlen(char * string){
    int i=0;
    while(string[i++]!=0);
    return i;
}

void printBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    print(buffer);
}

void printDec(uint64_t value){
    printBase(value, 10);
}


//================================================================================================================================
// Number handling
//================================================================================================================================
//================================================================================================================================
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}


//================================================================================================================================
// Sleep
//================================================================================================================================
static unsigned long ticks = 0;

void timer_handler() {
  ticks++;
}

int ticks_elapsed() { 
	return ticks; 
}

void sleep(int sec){
	unsigned long t0 = ticks_elapsed();
	while( (ticks_elapsed()-t0)/18 < sec );
}

//================================================================================================================================
// Clock
//================================================================================================================================

void formatTime(uint8_t *sec, uint8_t *min, uint8_t *hour) {
  *sec = _getSeconds();
  *min = _getMinutes();
  *hour = _getHours();

  // nos fijamos si hay que corregir el formato
  uint8_t format = _getDateTimeFormat();
  if ((!(format & 0x02)) && (*hour & 0x80)) { // si esta en 12hs, lo pasamos a 24
    *hour = ((*hour & 0x7F) + 12) % 24;
  }
  if (!(format & 0x04)) { // si esta en BCD pasamos a decimal
    *sec = (*sec & 0xF) + ((*sec / 16) * 10);
    *min = (*min & 0xF) + ((*min / 16) * 10);
    *hour = (*hour & 0xF) + ((*hour / 16) * 10);
  }

  // la hora viene en UTC, por lo que la asignamos en UTC-3 para Argentina
  // (ignoramos la lógica del caso con daylight savings)

  if (*hour >= 3)
    *hour -= 3;
  else {
    *hour = *hour + 21;
  }
}

void printTime(){

    uint8_t sec, min, hour;
    formatTime(&sec, &min, &hour);

	if(hour<10) printDec(0);
	printDec(hour);
	print(":");
	if(min<10) printDec(0);
	printDec(min);
	print(":");
	if(sec<10) printDec(0);
	printDec(sec);
	print("\n");
}

