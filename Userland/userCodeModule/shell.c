#include "include/userlib.h"
#include "include/userlibasm.h"
#include <stdint.h>

#define COMMANDS 4
#define VERT_SIZE 32
#define LINE_SIZE 63
#define BUFFER_SIZE 128

#define PROMPT_START ">"
#define ERROR_PROMPT "Unknown command: "

// Buffers
char screen_buffer[VERT_SIZE][LINE_SIZE];
char command_buffer[BUFFER_SIZE];
static char* commands[COMMANDS] = {"exit", "clear", "inc-size", "dec-size"};
char char_buffer[1];

// Cursors & flags
int command_cursor = 0;
int cursor_y = 0;
int cursor_x = 0;
int exit_command = 0;

// Important values
uint8_t font_size;
int rows_to_show;
int limit_index = VERT_SIZE - 1;
int line_size = LINE_SIZE;


// commands to do: help, resize, time, registers
// commands to shitpost: cowsay, ls, amogus?


int shell(){
    clearScreen();
    init_shell();
    write_out(PROMPT_START);

    while(!exit_command){
        if (read(char_buffer, 1) == 1){
            process_key(char_buffer[0]);
        }
        sleep_once();
    }

    clearScreen();

    return 0;
}


void process_key(char key){
    if (key == '\n'){

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
        cursor_x = mod(cursor_x - 1, line_size);

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
                    limit_index = VERT_SIZE/font_size - 1;
                    break;
                case 2:
                    // get second param
                    if (font_size == 3){
                        write_out("Font size max!\n");
                    }
                    else {
                        change_font(++font_size);
                        resize();
                    }
                    break;
                
                case 3:
                    // get second param
                    if (font_size == 1){
                        write_out("Font size minimum!\n");
                    }
                    else {
                        change_font(--font_size);
                        resize();
                    }
                    break;
                
            }
            return;
        }
    }

    if (strlen(buffer) == BUFFER_SIZE){
        write_out("Buenas... una poesia?\n");
        return;
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

    for (int i = 1; i < rows_to_show; i++){
        
        int line_number = mod(i + (limit_index - rows_to_show + 1), VERT_SIZE);


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

        // el menos 1 es porq line_size es 1 mas que el maximo indice
        if (cursor_x == line_size - 1|| string[c] == '\n'){
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


void init_shell(){
    font_size = getFontSize();
    rows_to_show = VERT_SIZE/font_size;
}


void resize(){
    init_shell();
    line_size = LINE_SIZE/font_size;



    int from = mod(cursor_y - rows_to_show + 1, VERT_SIZE);
    int offset = 0;

    clearScreen();

    // voy a hacer un for auxiliar para no tener que shiftear mil veces
    for (int i = 0; i < rows_to_show; i++){
        if (screen_buffer[(from + i) % VERT_SIZE][0] == 0 || strlen(screen_buffer[(from + i) % VERT_SIZE]) > line_size)
            offset++;
    }

    // falta casi nada, lo tenes cocinado rey
    for (int i = 0; i < rows_to_show - offset - 1; i++){
        mat_copy(screen_buffer[(offset + from + i) % VERT_SIZE]);
        mat_copy("\n");
    }

}


void mat_copy(char* string){
    for (int c = 0; c < strlen(string)-1; c++){

        // el menos 1 es porq line_size es 1 mas que el maximo indice
        if (cursor_x == line_size - 1|| string[c] == '\n'){
            if (string[c] == '\n') 
                screen_buffer[cursor_y][cursor_x] = '\0'; // null terminate en caso de print
            else
                c--;    // se que parece raro pero esto esta para no escribir el \n en la tabla pero si los demas despues del shift
            cursor_x = 0;
        }
        // ese else c-- esta para que pueda entrar a este else con el ultimo caracter de cada linea
        else {
            screen_buffer[cursor_y][cursor_x++] = string[c];
        }
    }

    print(string);

}
