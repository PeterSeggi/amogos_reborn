#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/userlib.h"
#include <stdint.h>
#include <stddef.h>

#define COMMANDS 2

typedef enum Commands{
    PS,
    EXIT
}Commands;


void ps(void);
int sh(void);


int init_sh(){
    return create_process(&sh, 0, NULL);
}

pid_t init_ps(){
   return create_process(&ps, 0, NULL);
}

static char* commands[COMMANDS] = {"ps", "exit"};

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


//cosas de ps :D================================================3
char * get_process_status(State state){
    switch(state){
        case READY:
            return "READY";
        case RUNNING:
            return "RUNNING";
        case BLOCKED:
            return "BLOCKED";
        default:
            return "UNKOWN";
    }
}
char * get_process_foreground(uint8_t foreground){
    if(foreground==TRUE) return "YES";
    else return "NO";
}
//B=================================================D
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

void ps(){
    uint16_t process_amount = 0;
    ProcessView ** processes  = get_processes(&process_amount);
    char aux_aux[BUFFER_SIZE];
    if(!processes){
        print("No processes\n");
        print("amount:");
        uintToBase(process_amount, aux_aux, 10);
        print(aux_aux);
        print("\n");
        return;
    }
    print("PID\t | STATE\t | PRIORITY\t | RSP\t | RBP\t | RIP\t | FOREGROUND\t | PARENT\t | CHILDREN\n");
    char aux[BUFFER_SIZE];
    for(int i = 0; i<process_amount; i++){
        uintToBase(processes[i]->pid, aux, 10);
        print(aux);
        print("\t |");
        print(get_process_status(processes[i]->state));
        print("\t |");
        uintToBase(processes[i]->priority, aux, 10);
        print(aux);
        print("\t |");
        uintToBase(processes[i]->registers.rsp, aux, 16);
        print(aux);
        print("\t |");
        uintToBase(processes[i]->registers.rbp, aux, 16);
        print(aux);
        print("\t |");
        uintToBase(processes[i]->registers.rip, aux, 16);
        print(aux);
        print("\t |");
        print(get_process_foreground(processes[i]->foreground));
        print("\t |");
        uintToBase(processes[i]->fatherPid, aux, 10);
        print(aux);
        print("\t |");
        uintToBase(processes[i]->children_amount, aux, 10);
        print(aux);
        print("\n");

        my_free(processes[i]);
    }
    my_free(processes);
    exit();
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


void parse_command(const char *input, char *c1, char **argv, int *argc) {
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

    //COMIENZO DEL FIN
    for(int i=0; i<COMMANDS; i++){
        if(!strcmp(argv[0], commands[i])){
            switch(i){
                case 0:
                    pid_t psPid = init_ps();
                    waitpid(psPid);
                    break;
                case COMMANDS-1:
                    exit();
            }
        }
    }
    my_free(temp);
}
