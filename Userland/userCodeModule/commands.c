#include <stddef.h>
#include "include/commands.h"

extern char endOfBinary;
extern char bss;

uint64_t aux_mem_state[3];
int * aux_mem_pointer = NULL;
char aux[BUFFER_SIZE];
uint64_t aux_mem_state[3];

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
    print("PID | NAME\t| STATE\t | PRI\t|   RSP  |   RBP  | FOR | PAR | CHI |\n");
    char aux[BUFFER_SIZE];
    int dif;
    for(int i = 0; i<process_amount; i++){

        // pid
        uintToBase(processes[i]->pid, aux, 10);
        print(" ");
        print(aux);
        print("\t |");

        // name
        print(processes[i]->name);
        dif = 7 - strlen(processes[i]->name);
        for (int i = 0; i < dif; i++) print(" ");
        print("|");

        // state
        print(get_process_status(processes[i]->state));
        dif = 9 - strlen(get_process_status(processes[i]->state));
        for (int i = 0; i < dif; i++) print(" ");
        print("|");

        // priority
        uintToBase(processes[i]->priority, aux, 10);
        print("  ");
        print(aux);
        print("\t |");

        // rsp
        uintToBase(processes[i]->registers.rsp, aux, 16); 
        print(" ");
        print(aux);
        print(" |");

        // rbp
        uintToBase(processes[i]->registers.rbp, aux, 16);
        print(" ");
        print(aux);
        print(" |");

        // foreground
        print(" ");
        print(get_process_foreground(processes[i]->foreground));
        if (!processes[i]->foreground) print(" ");
        print(" |");

        // father
        uintToBase(processes[i]->fatherPid, aux, 10);
        print("  ");
        print(aux);
        print("  |");

        //children
        uintToBase(processes[i]->children_amount, aux, 10);
        print("  ");
        print(aux);
        print("  |\n");

        my_free(processes[i]);
    }
    my_free(processes);
    exit();
}



pid_t init_ps(int read_fd, int write_fd, boolean foreground){
    char * name = strdup("ps");
    return create_shiny_process(&ps, 1, &name, 4, FALSE, foreground, read_fd, write_fd);
}


void loop(int argc, char * argv[]){
    
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
    else{
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
    }
    

pid_t init_loop(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&loop, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void mem(int argc, char * argv[]){
    print("endOfBinary: ");
    uintToBase((uint64_t) &endOfBinary, aux, 16);
    print(aux);
    print("\n");
    print("bss: ");
    uintToBase((uint64_t) &bss, aux, 16);
    print(aux);
    print("\n");
    getMemState(aux_mem_state);
    print("Total: ");
    //byteUnit[0] = byteConverter(aux_mem_state);
    uintToBase(aux_mem_state[0], aux, 10);
    print(aux);
    //print(byteUnit);
    print("B\n");
    print("Disponible: ");
    //byteUnit[0] = byteConverter(aux_mem_state+1);
    uintToBase(aux_mem_state[1], aux, 10);
    print(aux);
    //print(byteUnit);
    print("B\n");
    print("Ocupada: ");
    //byteUnit[0] = byteConverter(aux_mem_state+2);
    uintToBase(aux_mem_state[2], aux, 10);
    print(aux);
    //print(byteUnit);
    print("B\n");
    exit();
}

pid_t init_mem(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&mem, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void malloc(int argc, char * argv[]){
    
    print("Probamos malloc(sizeof(int))\n");
    if(aux_mem_pointer) print("Eu, acordate del free antes, no?\n");
    else{
        getMemState(aux_mem_state);
        print("Pre-Ocupada: ");
        uintToBase(aux_mem_state[2], aux, 10);
        print(aux);
        print("B\n");
        aux_mem_pointer=(int *) my_malloc(sizeof(int));
        if(!aux_mem_pointer) print("No pudo guardar la memoria :(\n");
        else{
            *(aux_mem_pointer)=777;
            print("aux_mem_pointer = ");
            uintToBase((uint64_t) aux_mem_pointer, aux, 10);
            print(aux);
            print("\n");
            print("*(aux_mem_pointer) = ");
            uintToBase(*aux_mem_pointer, aux, 10);
            print(aux);
            print("\n");
            getMemState(aux_mem_state);
            print("Post-Ocupada: ");
            uintToBase(aux_mem_state[2], aux, 10);
            print(aux);
            print("B\n");
        }
    }
    exit();
}

pid_t init_malloc(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&malloc, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}


void free(int argc, char * argv[]){
    print("Probamos free(sizeof(int)\n");
    if(!aux_mem_pointer) print("Eu, acordate del malloc primero, no?\n");
    else{
        getMemState(aux_mem_state);
        print("Pre-Ocupada: ");
        uintToBase(aux_mem_state[2], aux, 10);
        print(aux);
        print("B\n");
        my_free(aux_mem_pointer);
        getMemState(aux_mem_state);
        print("Post-Ocupada: ");
        uintToBase(aux_mem_state[2], aux, 10);
        print(aux);
        print("B\n");
        aux_mem_pointer=NULL;//reset
    }
    exit();
}

pid_t init_free(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&free, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void block(int argc, char * argv[]){
    print("Probamos bloquear : )\n");
    print("Abrimos sem\n");
    sem_t * my_sem = sem_open("my_sem", 0);
    if(!my_sem){
        print("algo salio mal\n");
    }
    print("up(sem)\n");
    sem_up(my_sem);
    print("down(sem)\n");
    sem_down(my_sem);
    print("down(sem)\n");
    sem_down(my_sem);
    print("????\n");
    exit();
}

pid_t init_block(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&block, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void test_pipe(int argc, char * argv[]){
    print("Probamos pipes (literalmente anda a saber)\n");
    int aux_fds[2];
    if(pipe(aux_fds)){
        print("salio mal\n");
        exit();
    }
    if(write(aux_fds[1], "holi\0", strlen("holi\0"))<0){
        print("nao write\n");
        exit();
    }
    if(read_fd(aux_fds[0], aux, 128)<=0){
        print("nao read\n");
        exit();
    }

    print(aux);
    print("\n");

    print("before blocks");
    if(read_fd(aux_fds[0], aux, 128)<=0){
        print("nao read\n");
        exit();
    }
    print(aux);
    print("\n");
    exit();
}

pid_t init_pipe(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&test_pipe, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}
