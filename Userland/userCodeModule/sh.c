#include "include/sh.h"
#include "include/userlib.h"
#include <stdint.h>

int init_sh(){
    return create_process(&sh);
}

char* let = " ";
//char prompt_start[] = {127, 0};
char* prompt_start = "> ";
char command_buffer[BUFFER_SIZE];
int command_cursor = 0;

int sh(){

    clearScreen();
    print(prompt_start);
    while(1){
        if (read(let, 1) == 1){
            process(let[0]);
        }

    }
    return 0;
}


void process(char key){
    print(let);
    if (key == '\n') process_command();
    else if(command_cursor == BUFFER_SIZE - 1){
     return;
    }
    else if (key <= 126 && key >= 20){
        command_buffer[command_cursor++] = key;
    }
}


void process_command(){
    command_buffer[command_cursor] = 0;
    print(command_buffer);
    //strcpy(command_buffer, "");
    command_cursor = 0;
    print("\n");
    print(prompt_start);

}
