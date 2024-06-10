#include "include/sh.h"
#include "include/userlib.h"
#include <stdint.h>
#include <stddef.h>

int init_sh(int read_fd, int write_fd){
    char * name = strdup("shell");
    return create_shiny_process(&sh, 1, &name, 4, FALSE, TRUE, read_fd, write_fd);
}

char* let = " ";
//char prompt_start[] = {127, 0};
char* prompt_start = "> ";
char command_buffer[BUFFER_SIZE];
int command_cursor = 0;

// buffers para el parse_command
char c1_buf[BUFFER_SIZE];
char * argv1[BUFFER_SIZE / 3];
const char delim[2] = {32, 0};
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
    if (key == '\n') process_command();

    else if(command_cursor == BUFFER_SIZE - 1){
     return;
    }

    else if (key == '\b'){
        if (command_cursor){
            print(let);
            command_buffer[--command_cursor] = 0;
        }
    }
    else if (key <= 126 && key >= 20){
        print(let);
        command_buffer[command_cursor++] = key;
    }
}


void process_command(){
    print(let);
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

    char *token = strtok(temp, delim);
    int index = 0;

    if (token != NULL) {
        strcpy(c1, token);
    }

    while (token != NULL) {

        if (token[0] == '|'){
            token = strchr(input, '|');
        }

        
        a1[index] = strdup(token);
        if (a1[index] == NULL) {
            return;
        }
        index++;
        token = strtok(NULL, delim);
    }

    *a1_size = index;
    print("Command es: ");
    print(c1);
    print("\n");

    print("Argc es es: ");
    printDec(index);
    print("\n");

    print("Argv son: ");
    for (int i = 0; i < index; i++){
        print(argv1[i]);
        print("\n");
    }

    my_free(temp);
}
