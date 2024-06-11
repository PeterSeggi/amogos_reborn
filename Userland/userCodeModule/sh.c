#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/userlib.h"
#include <stdint.h>
#include <stddef.h>
#include "include/commands.h"

#define COMMANDS 12


int init_sh(int read_fd, int write_fd){
    char * name = strdup("shell");
    return create_shiny_process(&sh, 1, &name, 4, FALSE, TRUE, read_fd, write_fd);
}
static char* commands[COMMANDS] = {"ps","loop","mem","help","sleep","kill","nice","block","cat","wc","filter","exit"};

char* let = " ";
//char prompt_start[] = {127, 0};
char* prompt_start = "> ";
char command_buffer[BUFFER_SIZE];
int command_cursor = 0;

// buffers para el parse_command
char c1_buf[BUFFER_SIZE];
char * argv1[BUFFER_SIZE];
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


void parse_command(char *input, char *c1, char *argv[], int *argc) {
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
                    print("me duermo\n");
                    sleep(1,0);
                    break;

                case 5:     //kill
                    pid_t killPid = init_kill(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    waitpid(killPid);
                    break;

                case 6:     //nice
                    pid_t nicePid = init_nice(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    waitpid(nicePid);
                    break;

                case 7:     //block
                    pid_t blockPid = init_block(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    waitpid(blockPid);
                    break;

                case 8:     //cat
                    pid_t catPid = init_cat(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    waitpid(catPid);
                    break;

                case 9:     //wc
                    pid_t wcPid = init_wc(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    waitpid(wcPid);
                    break;

                case 10:    //filter
                    pid_t filterPid = init_filter(*argc, argv, pipe_out[1], pipe_in[0], foreground);
                    waitpid(filterPid);
                    break;
                case COMMANDS-1:
                    exit();
            }
        }
    }

    // aca tendria que cerrar los pipes si o si 
    my_free(temp);
}
