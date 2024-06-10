#include <stddef.h>
#include "include/commands.h"


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



pid_t init_ps(){
   return create_process(&ps, 0, NULL);
}


void loop(int argc, char * argv[]){
    char aux[BUFFER_SIZE];
    print("La cantidad de argumentos es: ");
    uintToBase(argc, aux, 10);
    print(aux);
    print("\n");
    print("Numero es: ");
    print(argv[0]);
    print("\n");
    if(argv[0][0] == '5'){
        print("por el culo te la hinco\n");
    }
    if(argc==0) exit();
    int i=0;
    while(i<10){
        print("hola proceso n: ");
        print(argv[0]);
        print("!\n");
        i++;
        sleep(1,0);
    }
    exit();
}

pid_t init_loop(int argc, char * argv[]){
    return create_process(&loop, argc, argv);
}