#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/userlib.h"
#include <stdint.h>
#include <stddef.h>
#include "include/commands.h"

#define COMMANDS 7


int init_sh(int read_fd, int write_fd){
    char * name = strdup("shell");
    return create_shiny_process(&sh, 1, &name, 4, FALSE, TRUE, read_fd, write_fd);
}
static char* commands[COMMANDS] = {"ps", "loop","mem", "help", "sleep", "clear", "exit"};

char* let = " ";
//char prompt_start[] = {127, 0};
char* prompt_start = "> ";
char command_buffer[BUFFER_SIZE] = {0};
int command_cursor = 0;

// buffers para el parse_command
char c1_buf[BUFFER_SIZE] = {0};
char * argv1[BUFFER_SIZE / 3] = {0};
const char delim[2] = {32, 0};
int argc1;
int ex;
int exited = 0;



void sh(){

    clearScreen();
    strcpy(command_buffer, "");
    strcpy(c1_buf, "");
    command_cursor = 0;
    print(prompt_start);

    while(read(let, 1) != -1){
        /*
        ex = read(let, 1);
        if (ex == -1) exited = 1;
        else process(let[0]);
        */
        process(let[0]);
    }

    exit();
}

void help(int argc, char * argv[]){
    print("Los comandos existentes son:\n");
    for(int i=0; i<COMMANDS; i++){
        print(commands[i]);
        print("\n");
    }
    exit();
}

pid_t init_help(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&help, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
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

    else if (key <= 126 && key >= 32){
        print(let);
        command_buffer[command_cursor++] = key;
    }
}


void process_command(){
    print(let);
    command_buffer[command_cursor] = 0;
    if (command_cursor == 0){
        print(prompt_start);
        return;
    }
    //print(command_buffer);
    //strcpy(command_buffer, "");
    command_cursor = 0;

    //if(command_buffer[0] == 'q') exit();

    parse_command(command_buffer, c1_buf, argv1, &argc1);
    command_cursor = 0;
    //print("\n");
    print(prompt_start);

}


void parse_command(char *input, char *c1, char **argv, int *argc) {
    boolean foreground = TRUE;
    if(input[strlen(input)-2]=='&'){
        foreground = FALSE;
    }
    //printDec(foreground==TRUE);
    //printDec(foreground==TRUE);
    //printDec(foreground==FALSE);
    
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
                    if(foreground==TRUE)waitpid(psPid);
                    break;

                case 1: //loop

                    pid_t loopPid = init_loop(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    if(foreground==TRUE)waitpid(loopPid);
                    break;

                case 2: //mem

                    pid_t memPid = init_mem(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    if(foreground==TRUE)waitpid(memPid);
                    break;

                case 3: //help

                    pid_t helpPid = init_help(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    if(foreground==TRUE)waitpid(helpPid);
                    break;

                case 4: //SLEEP
                    print("me duermo...");
                    sleep(1,0);
                    print(" me desperte! :D\n");
                    break;

                case 5: //CLEAR
                    clearScreen();
                    break;

                case COMMANDS-1:
                    my_free(temp);
                    pclose(pipe_in[0]);
                    pclose(pipe_out[0]);
                    pclose(pipe_in[1]);
                    pclose(pipe_out[1]);
                    exit();
            }
        }
    }

    // aca tendria que cerrar los pipes si o si 
    my_free(temp);
    pclose(pipe_in[0]);
    pclose(pipe_out[0]);
    pclose(pipe_in[1]);
    pclose(pipe_out[1]);
}
