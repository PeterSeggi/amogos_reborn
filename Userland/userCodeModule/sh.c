// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/userlib.h"
#include <stdint.h>
#include <stddef.h>
#include "include/commands.h"

#define COMMANDS 18

void failure_free_chars(char ** ptr_list, int size);

int init_sh(int read_fd, int write_fd){
    int argc = 1;
    char **argv = (char **) my_malloc(sizeof(char *) * argc);
    if(!argv) return -1;
    argv[0] = strdup("shell");

    int to_ret = create_shiny_process(&sh, argc, argv, 4, FALSE, TRUE, read_fd, write_fd);

    my_free(argv[0]);
    my_free(argv);
    return to_ret;
}
static char* commands[COMMANDS] = {"ps","loop","mem","help","sleep","kill","nice","block","cat","wc","filter", "phylos", "test_sync", "test_proc", "test_prio", "test_mm", "clear", "exit"};

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
int exited = 0;



void sh(){

    exited = 0;
    clearScreen();
    strcpy(command_buffer, "");
    strcpy(c1_buf, "");
    command_cursor = 0;
    print(prompt_start);

    while(!exited && read(let, 1) != -1){
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



int parse_command(const char *input, int r_fd, int w_fd, boolean foreground) {
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
        my_free(argv);
        return 0;
    }
    
    token = strtok(temp, delim);

    int i = 0;
    while (token != NULL) {
        argv[i] = strdup(token);
        if (argv[i] == NULL) {
            failure_free_chars(argv, i-1);
            my_free(argv);
            my_free(temp);
            return 0;
        }
        i++;
        token = strtok(NULL, delim); 
    }

    my_free(temp);

    pid_t c_pid;
    boolean found = FALSE;

    for(int i=0; i<COMMANDS && !found; i++){
        if(!strcmp(argv[0], commands[i])){
            found = TRUE;
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

                case 12:    //test_sync
                    c_pid = init_test_sync(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 13:    //test procs
                    c_pid = init_procs(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 14:    //test priority
                    c_pid = init_test_prio(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 15:    //test mm
                    c_pid = init_test_mm(argc, argv, r_fd, w_fd, foreground);
                    break;

                case 16: //CLEAR
                    clearScreen();
                    c_pid = 0;
                    break;

                case COMMANDS-1:
                    c_pid = 1;
                    break;

            }
        }
    }

    if (!found){
        print("Command: '");
        print(argv[0]);
        print("' not found\n");
        c_pid = 0;

    }

    for (int i = 0; i < argc; i++){
        my_free(argv[i]);
    }
    my_free(argv);
    return c_pid;
}



void command_wrapper(const char* input){
    boolean foreground = TRUE;
    boolean piped = FALSE;

    char c1[BUFFER_SIZE / 2];
    char c2[BUFFER_SIZE / 2];

    if(input[strlen(input)-1]=='&'){
        foreground = FALSE;
    }

    char *temp = strdup(input);
    if (temp == NULL) {
        return;
    }

    char *token = strtok(temp, "|");
    my_free(temp);
    if (token == NULL) {
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

        if (cpid1 == 1 || cpid2 == 1) exited = 1; 

        
        // cierro manualmente todos los pipes por si alguno de los comandos no cerro los suyos (builtin)
        pclose(pipe_in[0]);
        pclose(pipe_mid[0]);
        pclose(pipe_out[0]);

        pclose(pipe_in[1]);
        pclose(pipe_mid[1]);
        pclose(pipe_out[1]);
    }

    else {
        int pipe_out[2] = {0};
        int pipe_in[2] = {0};

        if(pipe(pipe_out)) return; 
        if(pipe(pipe_in)) return; 

        pid_t cpid = parse_command(c1, pipe_out[0], pipe_in[1], foreground);
        
        if (cpid > 1 && foreground) waitpid(cpid); 

        pclose(pipe_in[0]);
        pclose(pipe_out[0]);

        pclose(pipe_in[1]);
        pclose(pipe_out[1]);

        if (cpid == 1) exited = 1;

    }
}


void failure_free_chars(char ** ptr_list, int size){
  while(size>=0){
    my_free(ptr_list[size--]);
  }
}
