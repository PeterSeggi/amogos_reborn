#include "include/userlib.h"
#include "include/userlibasm.h"

#define COMMANDS 2
#define VERT_SIZE 32
#define LINE_SIZE 63
#define BUFFER_SIZE 128
#define FONT_SIZE 1

#define PROMPT_START "$ "
#define ERROR_PROMPT "Unknown command: "

// Buffers
char screen_buffer[VERT_SIZE][LINE_SIZE];
char command_buffer[BUFFER_SIZE];
static char* commands[COMMANDS] = {"exit", "clear"};
char char_buffer[1];

// Cursors & flags
int command_cursor = 0;
int cursor_y = 0;
int cursor_x = 0;
int exit_command = 0;

// Important indexes
int limit_index = VERT_SIZE - 1;
int rows_to_show = VERT_SIZE / FONT_SIZE;


// commands to do: help, resize, time, registers
// commands to shitpost: cowsay, ls, amogus?


int shell(){
    clearScreen();
    print(PROMPT_START);

    while(!exit_command){
        if (read(char_buffer, 1) == 1){
            process_key(char_buffer[0]);
        }
        miliSleep(10);
    }

    clearScreen();

    return 0;
}


void process_key(char key){
    if (key == '\n'){

        /*
        En orden quiero:
        - Agregar \0 a ambos buffers
        - Shift 
            + check si esta en el limit
            + reescribo desde limit - rows_shown y cambio el limite a uno para abajo
        - cursor_y va para abajo o loopea (es circular) 
        - Meto un enter
        - proceso el comando 
        - copio y printeo el start 
         */

        command_buffer[command_cursor] = '\0';

        write_out("\n");
        process_command(command_buffer);

        command_cursor = 0;
        
        write_out(PROMPT_START);
        return;
    }

    if (key == '\b'){
        if (!command_cursor)
            return;

        command_cursor--;
        cursor_x = mod(cursor_x - 1, LINE_SIZE);

        // aca va printChar y no write_out porq es un caso especial
        printChar(key);
        return;
    }

    // a partir de aca si esta lleno el buffer nos vamos
    if (command_cursor == BUFFER_SIZE - 1) 
        return;

    else {
        command_buffer[command_cursor++] = key;
        write_out(char_buffer);
    }
}

void process_command(char* buffer){
   if (buffer[0] == '\0'){
        return;
    }
   for(int i = 0; i < COMMANDS; i++){
        if (!strcmp(buffer, commands[i])){
            switch (i) {
                case 0:
                    exit_command = 1;
                    break;
                case 1:
                    clearScreen(); 
                    cursor_y = 0;
                    cursor_x = 0;
                    limit_index = VERT_SIZE/FONT_SIZE - 1;
                    break;
            }
            return;
        }
    }

    // En caso de no encontrar hacemos esto
    cursor_x = 0;
    write_out(ERROR_PROMPT);
    write_out(buffer);
    write_out("\n");
    return;
}

void shift(){
    clearScreen();

    for (int i = 1; i < rows_to_show - 1; i++){
        
        int line_number = mod(limit_index - rows_to_show + i + 1, VERT_SIZE);


        print(screen_buffer[line_number]);
        if (i != rows_to_show - 1)
            print("\n");
    }
}

int check_shift(){
    int shifted = 0;
    if (cursor_y == limit_index){
        shift();
        shifted = 1;
        limit_index = (limit_index + 1) % VERT_SIZE;
    }

    cursor_y = (cursor_y + 1) % VERT_SIZE;
    return shifted;
}


void write_out(char* string){
    for (int c = 0; c < strlen(string)-1; c++){
        if (cursor_x == LINE_SIZE - 1 || string[c] == '\n'){
            if (string[c] == '\n') 
                screen_buffer[cursor_y][cursor_x] = '\0'; // null terminate en caso de print
            else
                c--;    // se que parece raro pero esto esta para no escribir el \n en la tabla pero si los demas despues del shift
            check_shift();
            cursor_x = 0;
        }
        // ese else c-- esta para que pueda entrar a este else con el ultimo caracter de cada linea
        else {
            screen_buffer[cursor_y][cursor_x++] = string[c];
        }
    }

    print(string);
}