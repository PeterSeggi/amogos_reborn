#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/file_descriptors.h"

static char buffer[64] = {'0'};
static char* char_buffer = " ";

//================================================================================================================================
// Writting
//================================================================================================================================
//================================================================================================================================
void printChar(char charToPrint){
    buffer[0] = charToPrint;
    print(buffer);
}

void print(char * string){
    _print(STDOUT, string, strlen(string));
}

void printCant(char* string, int cant){
    _print(STDOUT, string, cant);
}

void printError(char * string){
    _print(STDERROR, string, strlen(string));
}

void clearScreen(){
    _print(STDOUT, "\033[J", 3);
}

int read(char* buffer, int length){
    return _read(STDIN, buffer, length);
}

int readRaw(char* buffer, int length){
    return _read(STDKEYS, buffer, length);
}

void printBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    print(buffer);
}

void printDec(uint64_t value){
    printBase(value, 10);
}

void printHex(uint64_t value){
    printBase(value, 16);
}




//================================================================================================================================
// General use
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

int strcmp(const char *str1, const char *str2){
    while (*str1 && (*str1 == *str2)){
        str1++;
        str2++;
    }

    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

int strlen(char * string){
    int i=0;
    while(string[i++]!=0);
    return i;
}

void strcpy(char *destination, const char *source) {
    while (*source != '\0') {
        *destination++ = *source++;
    }
    *destination = '\0';
}


void halt(){
    _halt();
}

int mod(int val, int base){
    if (val < 0) return (val + base) % base;
    return val % base;
}
