#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/file_descriptors.h"
#include <stdint.h>
#include <stddef.h>

static char buffer[64] = {'0'};
static char* char_buffer = " ";

//================================================================================================================================
// Writting
//================================================================================================================================
//================================================================================================================================
void printChar(const char charToPrint){
    buffer[0] = charToPrint;
    print(buffer);
}

void print(const char * string){
    _print(STDOUT, string, strlen(string));
}

void printCant(const char* string, int cant){
    if (strlen(string) < cant) cant = strlen(string);
    _print(STDOUT, string, cant);
}

void printError(const char * string){
    _print(STDERROR, string, strlen(string));
}

void clearScreen(){
    _print(STDOUT, "\033[J", 3);
}

void flushBuffer(){
    _print(STDOUT, "\033[C", 3);
}

void change_font(int size){
    char* msg = "\033[nF";
    msg[2] = size + '0';
    print(msg);
}

int read(char* buffer, int length){
    return _read(STDIN, buffer, length);
}

int readRaw(char* buffer, int length){
    return _read(STDKEYS, buffer, length);
}

int readLast(char* buffer, int length){
    return _read(STDLAST, buffer, length);
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

int strlen(const char * string){
    int i=0;
    while(string[i++]!=0);
    return i - 1;
}

void strcpy(char *destination, const char *source) {
    while (*source != '\0') {
        *destination++ = *source++;
    }
    *destination = '\0';
}


int mod(int val, int base){
    if (val < 0) return (val + base) % base;
    return val % base;
}

char *strchr(const char *str, int c) {
    // Iterate through the string
    while (*str) {
        if (*str == (char)c) {
            return (char *)str; // Return the address of the first occurrence
        }
        str++;
    }

    // Check for the null terminator
    if (*str == (char)c) {
        return (char *)str;
    }

    return NULL; // Return NULL if the character is not found
}

char *strtok(char *str, const char *delim) {
    static char *last;
    char *start;
    
    if (str) {
        start = str;
    } else {
        start = last;
    }

    if (!start) {
        return NULL;
    }

    // Skip leading delimiters
    while (*start && strchr(delim, *start)) {
        start++;
    }

    if (!*start) {
        last = NULL;
        return NULL;
    }

    // Find the end of the current token
    char *token_end = start;
    while (*token_end && !strchr(delim, *token_end)) {
        token_end++;
    }

    // Null-terminate the token if needed
    if (*token_end) {
        *token_end = '\0';
        last = token_end + 1;
    } else {
        last = NULL;
    }

    return start;
}

char *strdup(const char *s) {
    uint64_t len = strlen(s) + 1;

    char *dup = (char *)my_malloc(len);
    if (dup == NULL) {
        return NULL; 
    }

    strcpy(dup, s);

    return dup; 
}

//================================================================================================================================
// Sleep
//================================================================================================================================

void sleep(uint32_t cant, uint32_t unidad){
	_sleep(cant, unidad);
}

void sleep_once(){
    _sleep(0, 1);
}

//================================================================================================================================
// Clock
//================================================================================================================================
void getClock(int *hrs, int *min, int *seg){
	_getClock(hrs, min, seg);
}

//================================================================================================================================
// Drawing
//================================================================================================================================
//================================================================================================================================
void getScreenData(uint16_t * screenHeight, uint16_t * screenWidth, uint8_t * fontSize, uint8_t * drawSize){
	_screenData(screenHeight,screenWidth,fontSize,drawSize);
}

int getFontSize(){
    // estos estan inicializados porq sino se rompe la funcion al querer escribir vacio
    uint16_t bufferHeight = 0;
    uint16_t bufferWeight = 0;
    uint8_t bufferDraw = 0;
    _screenData(&bufferHeight, &bufferWeight, (uint8_t*) char_buffer,&bufferDraw);
    return (int) char_buffer[0];
}


void draw(uint16_t * bitmap, uint32_t color, uint16_t height, uint64_t x, uint64_t y){
	_draw(bitmap, color, height, x, y);
}

void changeDrawSize(uint8_t newSize){
	_changeSize(newSize, 2);
}

//================================================================================================================================
// Registers
//================================================================================================================================

int getRegs(uint64_t regs[]){
    return _getRegs(regs);
}

//================================================================================================================================
// Beep!
//================================================================================================================================
void beep(uint32_t frequency, int duration){
    _beep(frequency, duration);
}

//================================================================================================================================
// Memory
//================================================================================================================================

void * my_malloc(uint32_t size){
    return _my_malloc(size);
}

void my_free(void * addr_to_free){
    _my_free(addr_to_free);
}

void getMemState(uint64_t * states){
    _getMemState(states);
}

char byteConverter(uint64_t * amount){
    uint64_t aux = (((((*amount)/1024)/1024)/1024)/1024);
    if(aux){
        *amount = aux;
        return 'T';
    }
    aux = (((((*amount)/1024)/1024)/1024));
    if(aux){
        *amount = aux;
        return 'G';
    }
    aux = (((*amount)/1024)/1024);
    if(aux){
        *amount = aux;
        return 'M';
    }
    aux = ((*amount)/1024);
    if(aux){
        *amount = aux;
        return 'K';
    }
    else return '\0';
}

//================================================================================================================================
// Processes
//================================================================================================================================

ProcessView ** get_processes(uint16_t * proc_amount){
    return _get_processes(proc_amount);
}

int create_process(void * function, int argc, char **argv){
    return _create_process(function, argc, argv);
}

int create_shiny_process(void * function, int argc, char ** argv, int priority, boolean orphan, boolean foreground, uint16_t stdin, uint16_t stdout){
    CreateArguments* args = (CreateArguments *)my_malloc(sizeof(CreateArguments));
    if (!args) return -1;

    args->priority=priority;
    args->orphan=orphan;
    args->foreground=foreground;
    args->stdin=stdin;
    args->stdout=stdout;

    pid_t cpid = _create_shiny_process(function, argc, argv, args);
    my_free(args);
    return cpid;
}

int waitpid(pid_t pid){
    return _waitpid(pid);
}

void kill(pid_t pid){
    _kill(pid);
}

void kill_children(pid_t pid){
    _kill_children(pid);
}

void exit(void){
    _exit();
}

void nice(pid_t pid, int priority){
    _change_proc_priority(pid, priority);
}

void block_proc(pid_t pid){
    _block_proc(pid);
}

pid_t get_pid(){
    return _get_pid();
}

void yield(){
    _yield();
}

//================================================================================================================================
// Semaphores
//================================================================================================================================

sem_t * sem_open(const char *name, uint16_t value){
   return  _sem_open(name, value);
}

int sem_close(sem_t *sem){
    return _sem_close(sem);
}

int sem_up(sem_t *sem){
    return _sem_up(sem);
}

int sem_down(sem_t *sem){
    return _sem_down(sem);
}

//================================================================================================================================
// Pipes
//================================================================================================================================

int pipe(int pipefd[2]){
    return _pipe(pipefd);
}

int pclose(int fd){
    return _pclose(fd);
}

int peek_read_pipe(){
    return _peek_read_pipe(STDIN);
}

int read_fd(int fd, char *buffer, uint16_t length){
    return _read(fd, buffer, length);
}

int write(int fd, char *message, uint16_t length){
    return _write(fd, message, length);
}

int peek(int fd){
    return _peek(fd);
}

