#include "include/userlib.h"
#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/snake.h"
#include <stdint.h>
#include <stddef.h>

#define COMMANDS 21 //AGREGUE UNO TODO: VOLVER A 15
extern char endOfBinary;//ESTO TMB
extern char bss;//ESTO TMB
#define VERT_SIZE 32
#define LINE_SIZE 63
#define BUFFER_SIZE 128

// Esto es un "string" manual para poder imprimir el caracter 128 de nuestro font de kernel usando lsa funciones estandar
char PROMPT_START[] = {127, 0};
#define ERROR_PROMPT "Unknown command: "

// Buffers
char screen_buffer[VERT_SIZE][LINE_SIZE];
char command_buffer[BUFFER_SIZE];
static char* commands[COMMANDS] = {"exit", "clear", "inc-size", "dec-size", "time", "sleep", "infoSleep", "help", "milisleep", "nanosleep", "registers", "snake", "test-div", "test-invalid", "speak", "mem", "malloc", "free", "ps", "bt", "pipe"};
char char_buffer[1];


typedef enum {
    EXIT,
    CLEAR,
} COMMAND_TYPE;


// Cursors & flags
int command_cursor = 0;
int cursor_y;
int cursor_x;
int exit_command;

// Important values
uint8_t font_size;
int rows_to_show;
int limit_index = VERT_SIZE - 1;
int line_size = LINE_SIZE;

int hrs = 0;
int min = 0; 
int sec = 0;

char aux[128];
int cantRegs = 18;
uint64_t regs[18];
char* regsNames[18] = {"rax:", "rbx:", "rcx:", "rdx:", "rsi:", "rdi:", "rbp:", "rsp:", "r8:", "r9:",
                       "r10:", "r11:", "r12:", "r13:", "r14:", "r15:", "rip:", "rflags:"};

                       
uint64_t aux_mem_state[3];
char byteUnit[2]={0};
int * aux_mem_pointer = NULL;

int sh(){
    cursor_x = 0;
    cursor_y = 0;
    exit_command = 0;

    clearScreen();
    while(1){

    }
    /*
    init_shell();
    write_out(PROMPT_START);


    while(!exit_command){
        if (read(char_buffer, 1) == 1){
            process_key(char_buffer[0]);
        }
        //sleep_once();
    }

    clearScreen();

    */
    return 0;
}


