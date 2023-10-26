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
// Drawing
//================================================================================================================================
//================================================================================================================================

void draw(uint16_t * bitmap, uint32_t color, uint16_t height, uint64_t x, uint64_t y){
	_draw(bitmap, color, height, x, y);
}