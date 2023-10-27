#include "include/userlib.h"
#include "include/userlibasm.h"

#define VERT_SIZE 32
#define LINE_SIZE 64

char command_buffer[VERT_SIZE][LINE_SIZE];
int cursor_y = 0;
int cursor_x = 0;
int exit_command = 0;

char* prompt_start = "$ ";

int shell(){
    clearScreen();

    while(!exit_command){
        if (read(command_buffer[cursor_y] + cursor_x, 1) == 1){
            print(command_buffer[cursor_y]);
            cursor_x++;
        }
    }
}
