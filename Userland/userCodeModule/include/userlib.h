#ifndef _OURLIB_H_
#define _OURLIB_H_

#include <stdint.h>

typedef enum boolean{
    FALSE,
    TRUE
}boolean;

typedef struct CreateArguments{
    int priority;
    boolean orphan;
    boolean foreground;
    uint16_t stdin;
    uint16_t stdout; 
}CreateArguments;
//================================================================================================================================
// Writting
//================================================================================================================================
//================================================================================================================================
int write(int fd, char *message, uint16_t length);

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
// Devuelve el string partiendo de un char 
//================================================================================================================================
char *strchr(const char *str, int c);

//================================================================================================================================
// Parte un string con un delimitador 
//================================================================================================================================
char *strtok(char *str, const char *delim);

//================================================================================================================================
// Hace malloc y devuelve una copia de s 
//================================================================================================================================
char *strdup(const char *s);

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

//********************************************************************************************************************************
//********************************************************************************************************************************

//================================================================================================================================
// Speaker
//================================================================================================================================

//================================================================================================================================
// Hace un beep! a la frecuencia y duración dada
//================================================================================================================================
void beep(uint32_t frequency, int duration);

//================================================================================================================================
// Memory
//================================================================================================================================

void * my_malloc(uint16_t size);

void my_free(void * addr_to_free);

void getMemState(uint64_t * states);

char byteConverter(uint64_t * amount);

//================================================================================================================================
// Processes
//================================================================================================================================

typedef int pid_t;


typedef enum State{
    READY,
    RUNNING,
    BLOCKED
}State;

typedef struct Registers{
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rip;
} Registers;

typedef struct ProcessView{
    unsigned int memory_size;
    pid_t pid;
    int priority;
    State state;
    Registers registers;
    boolean foreground;
    pid_t fatherPid;
    int children_amount;
    char * name;
}ProcessView;

typedef struct st_fds{
    uint16_t stdin;
    uint16_t stdout; 
}st_fds;

ProcessView ** get_processes(uint16_t * proc_amount);
int create_process(void * function, int argc, char **argv);
int create_shiny_process(void * function, int argc, char ** argv, int priority, boolean orphan, boolean foreground, uint16_t stdin, uint16_t stdout);
int waitpid(pid_t pid);
void kill(pid_t pid);
void exit(void);
void nice(pid_t pid);
void block_proc(pid_t pid);
pid_t get_pid();

//================================================================================================================================
// Semaphores
//================================================================================================================================

typedef struct sem_t sem_t;

sem_t * sem_open(const char *name, uint16_t value);
int sem_close(sem_t *sem);
int sem_up(sem_t *sem);
int sem_down(sem_t *sem);

//================================================================================================================================
// Pipes
//================================================================================================================================

int pipe(int pipefd[2]);
int pclose(int fd);
int peek_read_pipe();

int read_fd(int fd, char *buffer, uint16_t length);
int peek(int fd);

#endif
