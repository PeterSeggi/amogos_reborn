#include <stddef.h>
#include "include/commands.h"
#include "include/phylo.h"
#include "include/test_util.h"

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

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


void command_kill(int argc, char * argv[]){

    //se asume argv[1] como pid
    pid_t myPid = *argv[1]-'0'; 
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
    pid_t myPid = *argv[1]-'0';
    nice(myPid);    
    exit();
}

pid_t init_nice(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&command_nice, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void command_block(int argc, char * argv[]){
    pid_t myPid = *argv[1]-'0'; 
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
    //no se como accede un proceso a su propio stdin
    print(argv[1]);
    exit();
}

pid_t init_cat(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&command_block, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void wc(int argc, char * argv[]){
    //no se como accede un proceso a su propio stdin
    int count = 0;
    int offset = 0;
    while(argv[1]!=NULL && argv[1][offset]!='\0'){
        if(argv[1][offset]==' ') count++;
        offset++;
    }
    print("the count is: ");
    uintToBase(count, aux, 10);
    print(aux);
    print("\n");
    exit();
}

pid_t init_wc(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&wc, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void filter(int argc, char * argv[]){
    if (argc != 2) {
        print("Uso: filter <string>\n");
        exit();
    }
    char * filtered = strdup(argv[1]);
    int length = strlen(argv[1]);
    int j = 0;
    for (int i = 0; i < length; ++i) {
        char c = argv[1][i];
        // chequeo vocal
        if (!(c == 'a' || c == 'A' || c == 'e' || c == 'E' || c == 'i' || c == 'I' ||
              c == 'o' || c == 'O' || c == 'u' || c == 'U')) {
            filtered[j++] = c;
        }
    }
    filtered[j] = '\0'; // Null-terminate the filtered string
    print(filtered);
    print("\n");
    my_free(filtered);
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

//================TEST DE CATEDRA====================

void mem_test(int argc, char ** argv){
    test_mm(argc, argv);
    exit();
}

pid_t init_mm(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&mem_test, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void proc_test(int argc, char ** argv){
    test_processes(argc, argv);
    exit();
}

pid_t init_procs(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&proc_test, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}

void test_prio(int argc, char *argv[], int read_fd, int write_fd, boolean foreground) {
  int64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++){
    char * procName = "endless_loop_print";
    pids[i] = create_shiny_process(&endless_loop, 0, &procName, DEFAULT_PRIORITY, FALSE, foreground, read_fd, write_fd);
  }
  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i]); 

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block_proc(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i]);

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    (pids[i]);

  bussy_wait(WAIT);
  print("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
}

pid_t init_priority(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&test_prio, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}


//------TEST DE SINCRONIZACION------
void sync_test(int argc, char ** argv){
    test_sync(argc, argv);
    exit();
}

pid_t init_sync(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&sync_test, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}


//-------TEST PROCESSES--------
void test_my_processes(int argc, char ** argv){
    test_processes(argc, argv);
    exit();
}

pid_t init_sync(int argc, char * argv[], int read_fd, int write_fd, boolean foreground){
    boolean orphan = FALSE;
    return create_shiny_process(&test_my_processes, argc, argv, DEFAULT_PRIORITY, orphan, foreground, read_fd, write_fd);
}


