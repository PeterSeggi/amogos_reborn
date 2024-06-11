#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/userlib.h"
#include <stdint.h>
#include <stddef.h>
#include "include/commands.h"

#define COMMANDS 14

int init_sh(int read_fd, int write_fd){
    char * name = strdup("shell");
    return create_shiny_process(&sh, 1, &name, 4, FALSE, TRUE, read_fd, write_fd);
}
static char* commands[COMMANDS] = {"ps","loop","mem","help","sleep","kill","nice","block","cat","wc","filter", "phylo", "clear", "exit"};

char* let = " ";
//char prompt_start[] = {127, 0};
char* prompt_start = "> ";
char command_buffer[BUFFER_SIZE] = {0};
int command_cursor = 0;

// buffers para el parse_command
char c1_buf[BUFFER_SIZE] = {0};
char * argv1[BUFFER_SIZE] = {0};
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
    command_cursor = 0;

    command_wrapper(command_buffer);
    command_cursor = 0;
    print(prompt_start);

}



int parse_command(char *input, int r_fd, int w_fd, boolean foreground) {
    int argc = 0;
    
    char *temp = strdup(input);
    if (temp == NULL) {
        return 0;
    }
    
    char *token = strtok(temp, delim);

    while (token != NULL) {
        argc++;
        token = strtok(NULL, delim); 
    }

    my_free(temp);


    // hago el strtok dos veces para tener bien argc


    char ** argv = (char **) my_malloc(sizeof(char *) * argc);
    temp = strdup(input);
    if (temp == NULL) {
        return 0;
    }
    
    token = strtok(temp, delim);

    int i = 0;
    while (token != NULL) {
        argv[i] = strdup(token);
        if (argv[i] == NULL) {
            return 0;
        }
        i++;
        token = strtok(NULL, delim); 
    }

    my_free(temp);

    pid_t c_pid;

    for(int i=0; i<COMMANDS; i++){
        if(!strcmp(argv[0], commands[i])){
            switch(i){
                case 0: //ps

                    c_pid = init_ps(r_fd, w_fd, foreground);
                    break;

                case 1: //loop

                    c_pid = init_loop(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 2: //mem

                    c_pid = init_mem(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 3: //help

                    c_pid = init_help(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 4: //SLEEP
                    print("me duermo...");
                    sleep(1,0);
                    print(" me desperte! :D\n");
                    c_pid = 0;
                    break;


                case 5:     //kill
                    c_pid = init_kill(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 6:     //nice
                    c_pid = init_nice(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 7:     //block
                    c_pid = init_block(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 8:     //cat
                    c_pid = init_cat(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 9:     //wc
                    c_pid = init_wc(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 10:    //filter
                    c_pid = init_filter(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 11:    //phylo
                    c_pid = init_phylo(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 12: //CLEAR
                    clearScreen();
                    c_pid = 0;
                    break;

                case COMMANDS-1:
                    c_pid = 1;
            }
        }
    }

    for (int i = 0; i < argc; i++){
        my_free(argv[i]);
    }
    my_free(argv);
    return c_pid;
}



void command_wrapper(char* input){
    boolean foreground = TRUE;
    boolean piped = FALSE;

    char c1[BUFFER_SIZE / 2];
    char c2[BUFFER_SIZE / 2];

    if(input[strlen(input)-2]=='&'){
        foreground = FALSE;
    }

    char *temp = strdup(input);
    if (temp == NULL) {
        return;
    }

    char *token = strtok(temp, "|");
    if (token == NULL) {
        my_free(temp);
        return; 
    }

    strcpy(c1, token);

    token = strtok(NULL, "");
    if (token != NULL) {
        piped = TRUE;
        while (*token == ' ') {
            token++;
        }
        strcpy(c2, token);
    }
    else{
        c2[0] = 0;
    } 

    if (piped){

        int pipe_out[2] = {0};
        int pipe_in[2] = {0};
        int pipe_mid[2] = {0};

        if(pipe(pipe_out)) return; 
        if(pipe(pipe_in)) return; 
        if(pipe(pipe_mid)) return; 

        pid_t cpid1 = parse_command(c1, pipe_out[0], pipe_mid[1], FALSE);
        
        pid_t cpid2 = parse_command(c2, pipe_mid[0], pipe_in[1], foreground);

        if (cpid2 > 1 && foreground) waitpid(cpid2); 

        else{
            pclose(pipe_mid[0]);
            pclose(pipe_out[0]);
            pclose(pipe_mid[1]);
            pclose(pipe_out[1]);
        }

        if (cpid2 == 1) exit();


        //if (cpid1 > 1) waitpid(cpid1); 

        if (cpid1 <= 1){
            pclose(pipe_mid[0]);
            pclose(pipe_out[0]);
            pclose(pipe_mid[1]);
            pclose(pipe_out[1]);
        }

        if (cpid1 == 1) exit();
        

            
    }

    else {
        int pipe_out[2] = {0};
        int pipe_in[2] = {0};

        if(pipe(pipe_out)) return; 
        if(pipe(pipe_in)) return; 

        pid_t cpid = parse_command(c1, pipe_out[0], pipe_in[1], foreground);
        
        if (cpid > 1 && foreground) waitpid(cpid); 

        else{
            pclose(pipe_in[0]);
            pclose(pipe_out[0]);
            pclose(pipe_in[1]);
            pclose(pipe_out[1]);
        }

        if (cpid == 1) exit();

    }
}
