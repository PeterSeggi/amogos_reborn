#ifndef _OURLIB_H_
#define _OURLIB_H_

#include <stdint.h>

//================================================================================================================================
// Writting
//================================================================================================================================
//================================================================================================================================

//================================================================================================================================
// Imprime en pantalla un string
//================================================================================================================================
void print(char * string);

//================================================================================================================================
// Imprime en pantalla un char
//================================================================================================================================
void printChar(char charToPrint);

//================================================================================================================================
// Imprime en pantalla una determinada cantidad de chars de un string
//================================================================================================================================
void printCant(char* string, int cant);

//================================================================================================================================
// Imprime en pantalla un mensaje de error
//================================================================================================================================
void printError(char * string);

//================================================================================================================================
// Logica para imprimir en pantalla numeros
//================================================================================================================================
void printBase(uint64_t value, uint32_t base);
void printDec(uint64_t value);
void printHex(uint64_t value);

//================================================================================================================================
// Limpia la pantalla
//================================================================================================================================
void clearScreen();

//================================================================================================================================
// Cambia el tamaño de la font
//================================================================================================================================
void change_font(int size);

//================================================================================================================================
// Limpia el buffer de teclado
//================================================================================================================================
void flushBuffer();

//================================================================================================================================
// Obtiene el fontSize actual
//================================================================================================================================
int getFontSize();

//********************************************************************************************************************************
//********************************************************************************************************************************

//================================================================================================================================
// Sleep
//================================================================================================================================

void sleep(uint32_t cant, uint32_t unidad);

//================================================================================================================================
// Realiza un único halt
//================================================================================================================================
void sleep_once();

//********************************************************************************************************************************
//********************************************************************************************************************************

//================================================================================================================================
// Drawing
//================================================================================================================================
//================================================================================================================================

//================================================================================================================================
// Dibuja un bitmap de 16bit de ancho en pantalla
//================================================================================================================================
void draw(uint16_t * bitmap, uint32_t color, uint16_t height, uint64_t x, uint64_t y);

//================================================================================================================================
// Obtiene la información de dimensión de pantalla
//================================================================================================================================
void getScreenData(uint16_t * screenHeight, uint16_t * screenWidth, uint8_t * fontSize, uint8_t * drawSize);

//================================================================================================================================
// Cambia el tamaño de los dibujos
//================================================================================================================================
void changeDrawSize(uint8_t newSize);

//********************************************************************************************************************************
//********************************************************************************************************************************

//================================================================================================================================
// Reading
//================================================================================================================================
//================================================================================================================================

//================================================================================================================================
// Lee del buffer de input
//================================================================================================================================
int read(char* buffer, int length);

//================================================================================================================================
// Devuelve el scan code leido
//================================================================================================================================
int readRaw(char* buffer, int length);

//================================================================================================================================
// Devuelve el último caracter leido
//================================================================================================================================
int readLast(char* buffer, int length);


//********************************************************************************************************************************
//********************************************************************************************************************************

//================================================================================================================================
// General Purpose
//================================================================================================================================

//================================================================================================================================
// Devuelve la longitud de un string dado
//================================================================================================================================
int strlen(char * string);

//================================================================================================================================
// Copia un string en un destino
//================================================================================================================================
void strcpy(char *destination, const char *source);

//================================================================================================================================
// Compara dos strings
//================================================================================================================================
int strcmp(const char *str1, const char *str2);

//================================================================================================================================
// Logica para imprimir correctamente un numero dada su base
//================================================================================================================================
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

//================================================================================================================================
// Funcion modulo
//================================================================================================================================
int mod(int val, int base);

//********************************************************************************************************************************
//********************************************************************************************************************************

//================================================================================================================================
// Clock
//================================================================================================================================

//================================================================================================================================
// Devuelve la hora en los parametros dados
//================================================================================================================================
void getClock(int *hrs, int *min, int *seg);

//********************************************************************************************************************************
//********************************************************************************************************************************

//================================================================================================================================
// Registers
//================================================================================================================================

//================================================================================================================================
// Guarda el estado de los registros generales en el parametro dado
//================================================================================================================================
int getRegs(uint64_t regs[]);
#endif
