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
int limit_index = 0;
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
        halt();
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

        screen_buffer[cursor_y][cursor_x] = '\0';
        command_buffer[command_cursor] = '\0';
        check_shift();

        process_command(command_buffer);

        command_cursor = 0;
        cursor_x = 2; // dejo lugar para el prompt start
        
        strcpy(screen_buffer[cursor_y], PROMPT_START);
        print(PROMPT_START);
        return;
    }

    if (key == '\b'){
        if (!command_cursor)
            return;

        command_cursor--;
        cursor_x = mod(cursor_x - 1, LINE_SIZE);
        printChar(key);
        return;
    }

    // a partir de aca si esta lleno el buffer nos vamos
    if (command_cursor == BUFFER_SIZE - 1) 
        return;

    if (cursor_x == LINE_SIZE){
        in_line_shift();
        cursor_x = 0;
    }
    else {
        screen_buffer[cursor_y][cursor_x++] = key;
        command_buffer[command_cursor++] = key;
        printChar(key);
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
                    limit_index = 0;
                    break;
            }
            return;
        }
    }

    // En caso de no encontrar hacemos esto
    cursor_x = 0;

    strcpy(screen_buffer[(cursor_y) % VERT_SIZE], ERROR_PROMPT);
    cursor_x += strlen(ERROR_PROMPT) - 1; // ese - 1 saca el \0 del final del ERROR_PROMPT


    for (int c = 0; c < strlen(buffer); c++){
        if (cursor_x == LINE_SIZE - 1){
            print(screen_buffer[cursor_y]);
            in_line_shift();
            cursor_x = 0;
        }
        screen_buffer[cursor_y][cursor_x++] = buffer[c];
    }

    screen_buffer[cursor_y][cursor_x] = '\0';
    print(screen_buffer[cursor_y]);
    check_shift();
    return;
}

void shift(){
    clearScreen();
    for (int i = 1; i < rows_to_show; i++){
        int line_number = mod(limit_index - rows_to_show + i, VERT_SIZE);
        print(screen_buffer[line_number]);
        print("\n");
    }
}


void in_line_shift(){
    cursor_y = (cursor_y + 1) % VERT_SIZE;
    if (cursor_y == limit_index){
        clearScreen();
        int i;
        for (i = 1; i < rows_to_show - 1; i++){
            int line_number = mod(limit_index - rows_to_show + i, VERT_SIZE);
            print(screen_buffer[line_number]);
            print("\n");
        }
        print(screen_buffer[mod(limit_index - rows_to_show + i, VERT_SIZE)]);
    }
}



int check_shift(){
    int flagged = 0;
    cursor_y = (cursor_y + 1) % VERT_SIZE;
    if (cursor_y == limit_index){
        shift();
        flagged = 1;
        limit_index = (limit_index + 1) % VERT_SIZE;
    }
    else
        print("\n");
    return flagged;
}
