#include <ourlib.h>
#include <naiveConsole.h>

//================================================================================================================================
// dateTime functions for textmode
// ourasmlib for access to RTC
// naiveConsole for writing on screen
//================================================================================================================================

//================================================================================================================================

#define MILLENIUM 2000 	//cambiarlo si llega a ser necesario
#define CENTURY 0	//lo usamos para calcular la fecha completa
char * dayNames[]={"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};

void formatTime(uint8_t* sec, uint8_t* min, uint8_t* hour){
	*sec = _getSeconds();
	*min = _getMinutes();
	*hour = _getHours();

//nos fijamos si hay que corregir el formato
	uint8_t format = _getDateTimeFormat();
	if((!(format & 0x02)) && (*hour & 0x80)){	//si esta en 12hs, lo pasamos a 24
		*hour = ((*hour & 0x7F) + 12) % 24;
	}
	if(!(format & 0x04)){	//si esta en BCD pasamos a decimal
		*sec = (*sec & 0xF) + ((*sec/16)*10);
		*min = (*min & 0xF) + ((*min/16)*10);
		*hour = (*hour & 0xF) + ((*hour/16)*10);
	}

//la hora viene en UTC, por lo que la asignamos en UTC-3 para Argentina (ignoramos la lógica del caso con daylight savings)

	if(*hour>=3) *hour -=3;
	else{
		*hour = *hour + 21;
	}
}

void formatDate(uint8_t* dayWeek, uint8_t* dayMonth, uint8_t* month, uint16_t* year){
	*dayWeek = _getDayWeek();
	*dayMonth = _getDayMonth();
	*month = _getMonth();
	*year = _getYear();	//solo trae los dos ultimos numeros
	uint8_t hour = _getHours();

//nos fijamos si hay que corregir el formato
	uint8_t format = _getDateTimeFormat();
	if(!(format & 0x04)){	//si esta en BCD pasamos a decimal (dayWeek no hace falta porque siempre es entre 1-7)
		*dayMonth = (*dayMonth & 0xF) + ((*dayMonth/16)*10);
		*month = (*month & 0xF) + ((*month/16)*10);
		*year = (*year & 0xF) + ((*year/16)*10);
	}

//colocamos completo el año
	*year += MILLENIUM;
	*year += CENTURY;

	//esta en UTC, por lo que tengo que corregir la fecha segun hora, etc.
	if(hour<3){
		if(*dayMonth==1){//caso cambio de mes
			if(*month==1){//caso cambio de año
				*year=*year-1;
				*month=12;
			}
			else *month=*month-1;
			*dayMonth=calculateMonthLastDay(*month, *year);
		}
		else *dayMonth=*dayMonth-1;
		*dayWeek=((*dayWeek+5)%7)+1; //siempre va a ser el anterior
	}
}

//calcula el ultimo dia del mes dado
uint8_t calculateMonthLastDay(uint8_t month, uint16_t year){
	if(month==2){//hay que fijarse si es bisiesto
		if( (year%4==0) || (year%100!=0)  || (year%400==0) ) return 29;
		else return 28;
	}
	else{
		if((month%2==0 && month<8) || (month%2==1 && month>8)) return 30;
		else return 31;
	}
}

void printClock(){
	uint8_t dayWeek, dayMonth, month;
	uint16_t year;
	uint8_t sec, min, hour;
	formatDate(&dayWeek, &dayMonth, &month, &year);
	formatTime(&sec, &min, &hour);

	ourPrintSpace(dayNames[dayWeek-1], 66, 0);
	ourPrintSpace(" ", 69, 0);
	if(dayMonth<10){
		ourPrintDecSpace(0, 70, 0);
		ourPrintDecSpace(dayMonth, 71, 0);
	}
	else ourPrintDecSpace(dayMonth, 70, 0);
	ourPrintSpace("/", 72, 0);
	if(month<10){
		ourPrintDecSpace(0, 73, 0);
		ourPrintDecSpace(month, 74, 0);
	}
	else ourPrintDecSpace(month, 73, 0);
	ourPrintSpace("/", 75, 0);
	ourPrintDecSpace(year, 76, 0);

	if(hour<10){
		ourPrintDecSpace(0, 72, 1);
		ourPrintDecSpace(hour, 73, 1);
	}
	else ourPrintDecSpace(hour, 72, 1);
	ourPrintSpace(":", 74, 1);
	if(min<10){
		ourPrintDecSpace(0, 75, 1);
		ourPrintDecSpace(min, 76, 1);
	}
	else ourPrintDecSpace(min, 75, 1);
	ourPrintSpace(":", 77, 1);
	if(sec<10){
		ourPrintDecSpace(0, 78, 1);
		ourPrintDecSpace(sec, 79, 1);
	}
	else ourPrintDecSpace(sec, 78, 1);
}

void printDateTime(){
	printDate();
	printTime();
}

void printDate(){
	uint8_t dayWeek, dayMonth, month;
	uint16_t year;
	formatDate(&dayWeek, &dayMonth, &month, &year);

	ncPrint("Date: ");
	ncPrint(dayNames[dayWeek-1]);
	ncPrint(" ");
	if(dayMonth<10) ncPrintDec(0);
	ncPrintDec(dayMonth);
	ncPrint("/");
	if(month<10) ncPrintDec(0);
	ncPrintDec(month);
	ncPrint("/");
	ncPrintDec(year);
	ncNewline();
}

void printTime(){
	uint8_t sec, min, hour;
	formatTime(&sec, &min, &hour);

	ncPrint("Time: ");
	if(hour<10) ncPrintDec(0);
	ncPrintDec(hour);
	ncPrint(":");
	if(min<10) ncPrintDec(0);
	ncPrintDec(min);
	ncPrint(":");
	if(sec<10) ncPrintDec(0);
	ncPrintDec(sec);
	ncNewline();
}
//================================================================================================================================

//================================================================================================================================
// keyboard functions for textmode
// naiveConsole for writing on screen
//================================================================================================================================
//================================================================================================================================