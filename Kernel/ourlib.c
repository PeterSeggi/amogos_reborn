#include <ourlib.h>
#include <naiveConsole.h>

//================================================================
// videoDriver functions for textmode
//================================================================

static uint8_t * currentVideo = (uint8_t*)0xB8000;

void ourPrintCharColor(char character, char color)
{
	*currentVideo = character;
	currentVideo++;
	*currentVideo = color;
	currentVideo++;
}

void ourPrintColor(const char * string, char color)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ourPrintCharColor(string[i],color);
}
//================================================================

//================================================================
// dateTime functions for textmode
// ourasmlib for access to RTC
// naiveConsole for writing on screen
//================================================================
#define MILLENIUM 2000 	//cambiarlo si llega a ser necesario
#define CENTURY 0	//lo usamos para calcular la fecha completa
char * dayNames[]={"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};

void printDateTime(){
	printDate();
	printTime();
}

void printDate(){
	uint8_t dayWeek = _getDayWeek();
	uint8_t dayMonth = _getDayMonth();
	uint8_t month = _getMonth();
	uint16_t year = _getYear();	//solo trae los dos ultimos numeros

//nos fijamos si hay que corregir el formato
	uint8_t format = _getDateTimeFormat();
	if(!(format & 0x04)){	//si esta en BCD pasamos a decimal (dayWeek no hace falta porque siempre es entre 1-7)
		dayMonth = (dayMonth & 0xF) + ((dayMonth/16)*10);
		month = (month & 0xF) + ((month/16)*10);
		year = (year & 0xF) + ((year/16)*10);
	}

//colocamos completo el año
	year += MILLENIUM;
	year += CENTURY;

	ncPrint("Date: ");
	ncPrint(dayNames[dayWeek]);
	ncPrint(" ");
	ncPrintDec(dayMonth);
	ncPrint("/");
	ncPrintDec(month);
	ncPrint("/");
	ncPrintDec(year);
	ncNewline();
}

void printTime(){
	uint8_t sec = _getSeconds();
	uint8_t min = _getMinutes();
	uint8_t hour = _getHours();

//nos fijamos si hay que corregir el formato
	uint8_t format = _getDateTimeFormat();
	if(!(format & 0x02) && (hour & 0x80)){	//si esta en 12hs, lo pasamos a 24
		//TODO
	}
	if(!(format & 0x04)){	//si esta en BCD pasamos a decimal
		sec = (sec & 0xF) + ((sec/16)*10);
		min = (min & 0xF) + ((min/16)*10);
		hour = (hour & 0xF) + ((hour/16)*10);
	}

//la hora viene en UTC, por lo que la asignamos en UTC-3 para Argentina (ignoramos la lógica del caso con daylight savings)

	hour -=3;

	ncPrint("Time: ");
	ncPrintDec(hour);
	ncPrint(":");
	ncPrintDec(min);
	ncPrint(":");
	ncPrintDec(sec);
	ncNewline();
}
//================================================================