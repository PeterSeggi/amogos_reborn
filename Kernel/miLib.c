#include <miLib.h>
#include <naiveConsole.h>
 
static uint8_t * currentVideo = (uint8_t*)0xB8000;

/*
     ESTO SERIA EL DRIVER DEL EJ2, como usa el mismo currentVideo que
     ncPrintChar puede haber quilombo
*/
void ncPrintColor(const char * string, char color){
	int i;
	for(i=0; string[i]!=0; i++)
		ncPrintCharColor(string[i], color);
}
void ncPrintCharColor(char character, char color){
	*currentVideo = character;
	currentVideo++;
	*currentVideo = color;
	currentVideo++;
}

/*
    EJ3 
*/

void printRTC(){
    
    uint8_t hrs = rtcFunc(4);
    uint8_t min = rtcFunc(2);
    uint8_t sec = rtcFunc(0);

    uint8_t format = getFormat();
    if(!(format & 0x04)){	//si esta en BCD pasamos a decimal
		sec = (sec & 0xF) + ((sec/16)*10);
		min = (min & 0xF) + ((min/16)*10);
		hrs = (hrs & 0xF) + ((hrs/16)*10);
	}

    ncPrint("Hours:");
    ncPrintDec(hrs);
    ncNewline();
    ncPrint("Minutes:");
    ncPrintDec(min);
    ncNewline();
    ncPrint("Seconds:");
    ncPrintDec(sec);
    ncNewline();

   //22:55:1
   //22:55:17
   //osea q esta 3 hrs adelante de Arg
   //y da la casualidad q vino en formato 24hrs
}