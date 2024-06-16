#include <stddef.h>
#include "include/commands.h"
#include "include/phylo.h"
#include "include/test_util.h"
#include "include/userlib.h"


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
    else return "NO ";
}


void pid_to_string(int num, char str[]){
    int digit0 = num/100;
    int digit1 = num/10;
    int digit2 = num % 10;

    str[0] = '0' + digit0;
    str[1] = '0' + digit1;
    str[2] = '0' + digit2;
    str[3] = 0;

}

void ps(){
    uint16_t process_amount = 0;
    ProcessView ** processes  = get_processes(&process_amount);
    char aux_aux[BUFFER_SIZE] = {0};
    if(!processes){
        print("No processes\n");
        print("amount:");
        uintToBase(process_amount, aux_aux, 10);
        print(aux_aux);
        print("\n");
        return;
    }
    print("|PID| NAME | STATE | PRI |   RSP   |   RBP   | FOR | PAR | CHI |");
    char aux[BUFFER_SIZE] = {0};
    int dif;
    for(int i = 0; i<process_amount; i++){

        // pid
        pid_to_string(processes[i]->pid, aux);
        print("|");
        print(aux);
        print("|");

        // name
        printCant(processes[i]->name, 6);
        dif = 6 - strlen(processes[i]->name);
        for (int i = 0; i < dif; i++) print(" ");
        print("|");

        // state
        print(get_process_status(processes[i]->state));
        dif = 7 - strlen(get_process_status(processes[i]->state));
        for (int i = 0; i < dif; i++) print(" ");
        print("|");

        // priority
        uintToBase(processes[i]->priority, aux, 10);
        print("  ");
        print(aux);
        print("  |");

        // rsp
        uintToBase(processes[i]->registers.rsp, aux, 16); 
        print(" ");
        print(aux);
        print("h");
        print(" |");

        // rbp
        uintToBase(processes[i]->registers.rbp, aux, 16);
        print(" ");
        print(aux);
        print("h");
        print(" |");

        // foreground
        print(" ");
        print(get_process_foreground(processes[i]->foreground));
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
        if (processes[i]->children_amount < 10) print(" ");
        print(" |");

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
    int pid = get_pid();
    if(argc==0) exit();
    else{
        int i=0;
        while(1){
            print("hola proceso nro ");
            printDec(pid);
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


void command_kill(int argc, char ** argv){

    //se asume argv[1] como pid
    pid_t myPid = satoi(argv[1]); 

    if(myPid>2 && myPid<MAX_PROCESS_COUNT && myPid!=get_pid()){
        kill(myPid);
    }
    exit();
}

pid_t init_kill(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&command_kill, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void command_nice(int argc, char * argv[]){
    if(argc != 3) return;

    nice(satoi(argv[1]), satoi(argv[2]));    
    exit();
}

pid_t init_nice(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&command_nice, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void command_block(int argc, char * argv[]){
    pid_t myPid = satoi(argv[1]); 
    if(myPid>2 && myPid<MAX_PROCESS_COUNT){
        block_proc(myPid);
    }    
    exit();
}

pid_t init_block(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&command_block, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void cat(int argc, char * argv[]){
    char* let = strdup(" "); 
    while(read(let, 1) > 0){
        print(let);
    }
    my_free(let);
    exit();
}

pid_t init_cat(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&cat, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void wc(int argc, char * argv[]){
    int new_lines = 0;
    int words = 0;
    int bytes = 0;
    int first = TRUE;

    char* let = strdup(" "); 
    while(read(let, 1) > 0){
        
        if (first){
            words++;
            first = FALSE;
        }

        switch (let[0]) {
        case('\n'):
            new_lines++;

        case(' '):
            words++;

        default:
            bytes++;
        }
    }
    printDec(new_lines);
    print("    ");
    printDec(words);
    print("    ");
    printDec(bytes);
    print("\n");
    my_free(let);

    exit();
}

pid_t init_wc(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&wc, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void filter(int argc, char * argv[]){
    char* let = strdup(" "); 
    while(read(let, 1) > 0){
        if (!(let[0] == 'a' || let[0] == 'A' || let[0] == 'e' || let[0] == 'E' || let[0] == 'i' || let[0] == 'I' ||
              let[0] == 'o' || let[0] == 'O' || let[0] == 'u' || let[0] == 'U')) print(let);

    }
    my_free(let);
    exit();
}

pid_t init_filter(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&filter, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void the_real_phylo(int argc, char ** argv){
    phylo_command(argc, argv);
    exit();
}

pid_t init_phylo(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&the_real_phylo, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void sync_test(int argc, char ** argv){
    test_sync(argc, argv);
    exit();
}

pid_t init_test_sync(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&sync_test, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void prio_test(int argc, char ** argv, int read_fd, int write_fd){
    test_prio(argc, argv, read_fd, write_fd);
    exit();
}

pid_t init_test_prio(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&prio_test, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}


/*void mem_test(int argc, char ** argv){
    test_mm(argc, argv);
    exit();
}

pid_t init_mm(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&mem_test, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}
*/

void proc_test(int argc, char ** argv, int read_fd, int write_fd){
    test_processes(argc, argv, read_fd, write_fd);
    exit();
}

pid_t init_procs(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&proc_test, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}



