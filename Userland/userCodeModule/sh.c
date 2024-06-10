#include "include/sh.h"
#include "include/userlib.h"
#include <stdint.h>
#include <stddef.h>

int init_sh(){
    return create_process(&sh);
}

char* let = " ";
//char prompt_start[] = {127, 0};
char* prompt_start = "> ";
char command_buffer[BUFFER_SIZE];
int command_cursor = 0;

// buffers para el parse_command
char c1_buf[BUFFER_SIZE];
char * argv1[BUFFER_SIZE / 3];
int argc1;


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
    //print(command_buffer);
    //strcpy(command_buffer, "");
    command_cursor = 0;

    parse_command(command_buffer, c1_buf, argv1, &argc1);
    //print("\n");
    print(prompt_start);

}


void parse_command(const char *input, char *c1, char **a1, int *a1_size) {
    char *temp = strdup(input);
    if (temp == NULL) {
        return;
    }

    char *token = strtok(temp, " ");
    if (token != NULL) {
        strcpy(c1, token);
    }

    int index = 0;
    while ((token = strtok(NULL, " ")) != NULL) {
        a1[index] = strdup(token);
        if (a1[index] == NULL) {
            return;
        }
        index++;
    }

    *a1_size = index;
    print("Command es: ");
    print(c1);
    print("\n");

    print("Argc es es: ");
    printDec(index);
    print("\n");

    my_free(temp);
}
