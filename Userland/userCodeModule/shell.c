#include "include/userlib.h"
#include "include/userlibasm.h"

#define COMMANDS 2
#define VERT_SIZE 32
#define LINE_SIZE 65 // 64 de linea mas el null term al final

char command_buffer[VERT_SIZE][LINE_SIZE];
int cursor_y = 0;
int cursor_x = 0;
int exit_command = 0;

char* prompt_start = "$ ";
char char_buffer[1];

static char* commands[COMMANDS] = {"exit", "clear"};

int shell(){
    clearScreen();
    print(prompt_start);

    while(!exit_command){
        if (read(char_buffer, 1) == 1){
            process_key(char_buffer[0]);
        }
    }

    clearScreen();

    return 0;
}


void check_line(){
    if (cursor_x == LINE_SIZE){
        cursor_y++;
        cursor_x = 0;
    }


    if (cursor_y == VERT_SIZE){
        // shift goes here but pain
        return;
    }
}


void process_key(char key){
    if (key == '\n'){
        command_buffer[cursor_y][cursor_x] = '\0';
        print("\n");
        process_command();

        print(prompt_start);
        cursor_y++;
        cursor_x = 0;
        return;
    }

    if (key == '\b'){
        if (!cursor_x)
            return;

        cursor_x--; 
        printChar(key);
        return;
    }

    // a partir de aca si esta lleno la linea nos vamos
    if (cursor_x == LINE_SIZE - 1)
        return;

    else {
        command_buffer[cursor_y][cursor_x++] = key;
        printChar(key);
    }
}


void process_command(){
   if (command_buffer[cursor_y][0] == '\0')
        return;
   for(int i = 0; i < COMMANDS; i++){
        if (!strcmp(command_buffer[cursor_y], commands[i])){
            switch (i) {
                case 0:
                    exit_command = 1;
                    break;
                case 1:
                    clearScreen(); 
                    break;
            }
            return;
        }
    }

    print("Unknown command: ");
    print(command_buffer[cursor_y]);
    print("\n");
    return;
}
