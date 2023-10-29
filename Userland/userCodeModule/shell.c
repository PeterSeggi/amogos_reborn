#include "include/userlib.h"
#include "include/userlibasm.h"

#define COMMANDS 2
#define VERT_SIZE 32
#define LINE_SIZE 63
#define FONT_SIZE 1

char command_buffer[VERT_SIZE][LINE_SIZE];
int cursor_y = 0;
int cursor_x = 0;
int start_index = 0;
int input_index = 0;

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
        halt();
    }

    clearScreen();

    return 0;
}


void process_key(char key){
    if (key == '\n'){
        command_buffer[cursor_y][cursor_x] = '\0';
        print("\n");
        process_command();

        cursor_y++;
        check_shift();

        cursor_x = 2;
        strcpy(command_buffer[cursor_y], prompt_start);
        print(prompt_start);
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
    if (cursor_x == LINE_SIZE - 15)
        return;

    else {
        command_buffer[cursor_y][cursor_x++] = key;
        printChar(key);
    }
}

void process_command(){
   if (command_buffer[cursor_y][2] == '\0'){
        return;
    }
   for(int i = 0; i < COMMANDS; i++){
        if (!strcmp(command_buffer[cursor_y] + 2, commands[i])){
            switch (i) {
                case 0:
                    exit_command = 1;
                    break;
                case 1:
                    clearScreen(); 
                    cursor_y = 0;
                    cursor_x = 0;
                    start_index = 0;
                    break;
            }
            return;
        }
    }

    strcpy(command_buffer[(cursor_y + 1) % VERT_SIZE], "Unkown command: ");
    strcpy(command_buffer[(cursor_y + 1) % VERT_SIZE] + 15, command_buffer[cursor_y]  + 2); // ese +2 es para que no se guarde el prompt start en unknown command
    cursor_y++;
    check_shift();
    print(command_buffer[cursor_y]);

    print("\n");
    return;
}


void shift(){
    clearScreen();
    int rows_to_print = VERT_SIZE/FONT_SIZE;
    for (int i = 0; i < rows_to_print - 1; i++){
        print(command_buffer[(i + start_index + 1) % VERT_SIZE]);
        print("\n");
    }
    
}



void check_shift(){
    if (cursor_y == VERT_SIZE) cursor_y = 0;
    if (cursor_y == start_index){
        shift();
        start_index++;
        if (start_index == VERT_SIZE) start_index = 0;
    }
}





// hay que implementar esto 
int compare_command(int start_index, const char* command){
    int i = 0;
    int offset_com = 0;
    int offset = 0;
    while ((*command_buffer[start_index] + offset_com) && *command_buffer[start_index] + offset_com == command[offset]){
        if (offset_com == LINE_SIZE){
            offset_com = 0;
            start_index++;    
        } 
    }

    return *command_buffer[start_index] + offset_com - command[offset];
}
