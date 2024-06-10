#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/userlib.h"
#include <stdint.h>
#include <stddef.h>
#include "include/commands.h"

#define COMMANDS 10


int init_sh(int read_fd, int write_fd){
    char * name = strdup("shell");
    return create_shiny_process(&sh, 1, &name, 4, FALSE, TRUE, read_fd, write_fd);
}
static char* commands[COMMANDS] = {"ps", "loop","mem", "malloc", "free", "ps", "bt", "pipe", "cproc", "exit"};

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


    else if (key == '\b'){
        if (command_cursor){
            print(let);
            command_buffer[--command_cursor] = 0;
        }
    }

    else if(command_cursor == BUFFER_SIZE - 1){
     return;
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

    //if(command_buffer[0] == 'q') exit();

    parse_command(command_buffer, c1_buf, argv1, &argc1);
    command_cursor = 0;
    //print("\n");
    print(prompt_start);

}


void parse_command(const char *input, char *c1, char **argv, int *argc) {
    boolean foreground = TRUE;
    if(input[command_cursor-1]=='&'){
        foreground = FALSE;
    }
    printDec(foreground==TRUE);
    printDec(foreground==TRUE);
    printDec(foreground==FALSE);
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

        
        argv[index] = strdup(token);
        if (argv[index] == NULL) {
            return;
        }
        index++;
        token = strtok(NULL, delim); 
    }

    *argc = index;
    print("Command es: ");
    print(argv[0]);
    print("\n");

    print("Argc es es: ");
    printDec(*argc);
    print("\n");

    print("Argv son: ");
    for (int i = 0; i < index; i++){
        print(argv1[i]);
        print("\n");
    }

    int pipe_out[2] = {0};
    int pipe_in[2] = {0};

    if(pipe(pipe_out)) return; 
    if(pipe(pipe_in)) return; 


    //COMIENZO DEL FIN
    for(int i=0; i<COMMANDS; i++){
        if(!strcmp(argv[0], commands[i])){
            switch(i){
                case 0: //ps

                    pid_t psPid = init_ps(pipe_out[1], pipe_in[0], foreground);
                    waitpid(psPid);
                    break;

                case 1: //loop


                    pid_t loopPid = init_loop(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    waitpid(loopPid);
                    break;

                case 2:

                break;

                case 3:

                break;

                case 4:

                break;

                case 5:

                break;

                case 6:

                break;

                case 7: 
                
                break;

                case 8:

                break;

                case COMMANDS-1:    
                    exit();
            }
        }
    }

    // aca tendria que cerrar los pipes si o si 
    my_free(temp);
}
