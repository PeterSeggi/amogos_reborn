#include <stdint.h>
#include <mman.h>
#include <processManager.h>
#include <interrupts.h>
#include <lib.h>
#include <sem.h>


ProcessNode * delete_from_sched(ProcessNode * current, pid_t pid);
void delete_from_pcb(pid_t pid);
void destroyProcess(Process * process);
pid_t new_pid();
void unschedule(pid_t pid);
pid_t nextProcessInList(ProcessList * list, boolean wasRunning);
int getNextPriority();
SleepingProcess * remove_sleeper(SleepingProcess * current, pid_t pid);
void delete_sleeper(pid_t pid);
boolean add_child(pid_t fatherPid, pid_t childPid);
void free_argv(int argc, char ** argv);

//variables globales
ProcessTable * pcb = NULL;
PriorityArray * scheduler = NULL;
SleepingTable * sleepingTable = NULL;
ProcessList * foregroudnProcess = NULL;

int schedule_lock = 1;

//funciones publicas 
void initialize_pcb(void){
    int size = sizeof(ProcessTable);
    if(size%8!=0){size+=8-(size%8);}
    pcb = (ProcessTable *)my_malloc(size);
    pcb->size = 0;
    pcb->runningPid = 0;
    for(int i=0; i<MAX_PROCESS_COUNT; i++){
        pcb->processes[i] = NULL;
    }

    
    foregroudnProcess = (ProcessList *) my_malloc(sizeof(ProcessList));
    foregroudnProcess->firstProcess = NULL;
    foregroudnProcess->last = NULL;
    foregroudnProcess->current = NULL;
    foregroudnProcess->size = 0;
}

void executeProcess(uint64_t rip){
    ((void(*)())rip)();
}

void initializeScheduler(){
    int size = sizeof(PriorityArray);
    scheduler = (PriorityArray *)my_malloc(size);
    scheduler->size = 0;
    scheduler->runnableProcs = 0;
    scheduler->currentPriorityOffset = 0;    //señalizo que no tiene procesos corriendo aun
    int myPriorities[10] = {4, 4, 4, 4, 3, 3, 3, 2, 2, 1};
    for (int i = 0; i < 10; i++) {
        scheduler->priority[i] = myPriorities[i];
    }
    for(int i=0; i<5; i++){
        scheduler->list[i] = (ProcessList *)my_malloc(sizeof(ProcessList));
        if(scheduler->list[i]!=NULL){
            scheduler->list[i]->firstProcess=NULL;
            scheduler->list[i]->last=NULL;
            scheduler->list[i]->current=NULL;
            scheduler->list[i]->size=0;
        }
    }
    //createProcess(&my_main);   
    create_shiny_process((void *)0x400000, 0, NULL, 4, TRUE, FALSE, KEY_FD, VID_FD);
    create_shiny_process(&_idle, 0, NULL, 0, TRUE, FALSE, KEY_FD, VID_FD);     //proceso vigilante _hlt

    schedule_lock = 0;
    _idle();
}

Process * create_process(void * function, int argc, char ** argv){
    return create_shiny_process(function, argc, argv, DEFAULT_PRIORITY, FALSE, FALSE, KEY_FD, VID_FD);  //por defecto se crea con prioridad 4
}

void failure_free_chars(char ** ptr_list, int size){
  while(size>=0){
    my_free(ptr_list[size--]);
  }
}

Process * create_shiny_process(void * function, int argc, char ** argv, int priority, boolean orphan, boolean foreground, uint16_t stdin, uint16_t stdout){
    _cli();
    Process * process = (Process *)my_malloc(INITIAL_PROCESS_SIZE);
    if(process == NULL){
        return NULL;
    }
    process->memory_size = INITIAL_PROCESS_SIZE;
    process->state = READY;  
    process->foreground = foreground;
    process->children_amount = 0;
    process->argc = argc;
    if(process->argc){
        process->argv = (char **) my_malloc(sizeof(char *) * argc);
        if(!process->argv) return NULL;
        //TODO fREES AL LIBERAR EL PROCESO
        for(int i=0; i<argc; i++){
            process->argv[i] = (char *) my_malloc(sizeof(k_strlen(argv[i])));
            if(!process->argv[i]){
                failure_free_chars(process->argv, i-1);
                my_free(process->argv);
                return NULL;
            }
            k_strcpy(process->argv[i], argv[i]);
        }
        //por convencion argv[0] es el nombre del proceso, se asume null ttd
        k_strcpy(process->name, argv[0]);
    }
    
    

    //inicializa el stack
    process->registers.rbp = ( (uint64_t)process + INITIAL_PROCESS_SIZE ); 
    process->registers.rsp = process->registers.rbp;        //inicialmente stack vacio
    process->registers.rip = (uint64_t)function;  //direccion de la funcion a ejecutar
    process->registers.rsp = initializeStack((void *)process->registers.rsp, (void *)process->registers.rip, process->argc, process->argv); 
    process->pid = new_pid();       //cada nuevo proceso recibe el pid siguiente en orden natural
    process->stdin_fd = stdin;
    process->stdout_fd = stdout;
    
    if(process->pid == -1){
        free_argv(argc, argv);  
        my_free(process);  //libero recursos utlizados
        _sti();
        return NULL;
    }

    pid_t runningPid = pcb->runningPid;
    if(orphan==FALSE){            //los primeros 2 procesos no son hijos de nadie
        boolean added = add_child(runningPid, process->pid);   
        if(added==TRUE){
            process->fatherPid = runningPid; 
        }
        else{
            process->fatherPid = -1;
        }
    }
    else{
        process->fatherPid = -1;
    }
    for(int i=0; i<MAX_CHILDREN_COUNT; i++){
        process->children[i] = 0;
    }
    process->priority = priority;
    int result = pcb_append(process);
    if(result == 1){    //error en la creacion del proceso
        free_argv(argc, argv);  
        my_free(process);  //libero recursos utlizados
        _sti();
        return NULL;
    }

    int size = sizeof(ProcessNode);
    if(size%8!=0){size+=8-(size%8);}
    ProcessNode * node = (ProcessNode *)my_malloc(size);
    if(node == NULL){
        free_argv(argc, argv);  
        my_free(process);
        _sti();
        return NULL;
    }
    node->pid = process->pid;
    if(priority>4){
        free_argv(argc, argv);
        my_free(process);
        my_free(node);
        _sti();
        return NULL;
    }
    scheduler_add(process->pid, priority, node);  //agrega el proceso a la cola de scheduling con la prioridad deseada

    if (foreground == TRUE){
       add_foreground(process->pid);
    }
    
    _sti();
    return process;                     
}

void free_argv(int argc, char ** argv){
    for(int i=0; i<argc; i++){
        my_free(argv[i]);
    }
}


pid_t new_pid(){
    for(int i=1;i<MAX_PROCESS_COUNT;i++){
        if(pcb->processes[i] == NULL){
            return i;
        }
    }
    return -1;
}

int pcb_append(Process * process){ 
    
    if(pcb->size+1 >= MAX_PROCESS_COUNT || process==NULL){
        return 1;
    }
    pcb->processes[process->pid] = process;
    pcb->size++;
    return 0;
}

void change_priority(pid_t pid, int priority){
    if(pid<1 || pid>=MAX_PROCESS_COUNT || priority<0 || priority>4){
        return;
    }
    
    int oldPriority = pcb->processes[pid]->priority;
    if(oldPriority != priority){
        
        ProcessNode * newNode = (ProcessNode *)my_malloc(sizeof(ProcessNode));
        if(newNode == NULL){
            return;
        }
        unschedule(pid);
        newNode->pid = pid;
        scheduler_add(pid, priority, newNode);
        pcb->processes[pid]->priority = priority;
    }

}

boolean add_child(pid_t fatherPid, pid_t childPid){
    if(fatherPid<1 || fatherPid>=MAX_PROCESS_COUNT){
        return FALSE;
    }
    for(int i=0; i<MAX_CHILDREN_COUNT; i++){
        if(pcb->processes[fatherPid]->children[i]==0){
            pcb->processes[fatherPid]->children[i] = childPid;
            pcb->processes[fatherPid]->children_amount++;
            return TRUE;
        }
    }
    return FALSE;
}


pid_t get_pid(){
    return pcb->runningPid;
}
Process ** get_processes(){
    return pcb->processes;
}
int get_pcb_size(){
    return pcb->size;
}

boolean check_valid_pid(pid_t pid){
    return pcb->processes[pid]!=NULL;
}

State get_pid_state(pid_t pid){
    return pcb->processes[pid]->state;
}

void change_pid_priority(pid_t pid, int priority){
    pcb->processes[pid]->priority=priority;
}


//###############################--SCHEDULING ZONE--#########################################################

void * schedule(void * rsp){

    if (schedule_lock == 1){
        return rsp;
    } 


    int priority = scheduler->priority[scheduler->currentPriorityOffset];
    if(scheduler->list[priority]->current != NULL && pcb->runningPid > 0){ //si estoy en el primer proceso no me guardo el stack de kernel
        pcb->processes[pcb->runningPid]->registers.rsp = (uint64_t)rsp;           
    }
    pcb->runningPid = nextProcess();   //next process ignora los procesos bloqueados
    return (void *)pcb->processes[pcb->runningPid]->registers.rsp;
    //return rsp;
}


pid_t nextProcess(){
    int priority = scheduler->priority[scheduler->currentPriorityOffset];
    return nextProcessInList(scheduler->list[priority], TRUE);
}


pid_t nextProcessInList(ProcessList * list, boolean wasRunning){
    // si no tengo nada q correr voy directo a idle
    if(scheduler->runnableProcs == 0){
        // basically el pid del idle
        return scheduler->list[0]->firstProcess->pid;
    }
    if(list->size == 0){                //skippeo la lista vacia
        return nextProcessInList(scheduler->list[getNextPriority()], TRUE);   
    }
    if(list->current == NULL){
        list->current = list->firstProcess;     //se que size!=0 y por ende firstProcess!=NULL
        if(pcb->processes[list->current->pid]->state == BLOCKED){
            return nextProcessInList(list, TRUE);     //busco el siguiente en la misma lista
        }
        return list->current->pid;
    }
    if(list->current->next == NULL){      //ultimo proc de la lista
        if(wasRunning == FALSE){
            return list->current->pid;
        }
        list->current = NULL;            //reseteo lista
        return nextProcessInList(scheduler->list[getNextPriority()], TRUE);
    }
    list->current = list->current->next;
    if(pcb->processes[list->current->pid]->state == BLOCKED){
        return nextProcessInList(list, FALSE);     //busco el siguiente en la misma lista
    }
    return list->current->pid;
}





int getNextPriority(){
    scheduler->currentPriorityOffset = (scheduler->currentPriorityOffset+1)%10;
    return scheduler->priority[scheduler->currentPriorityOffset];
}





//###############################--END OF SCHEDULING ZONE--#########################################################

void listInsert(ProcessList * list, ProcessNode * process){
    if(process==NULL){  //no se admiten nodos vacios
        return;
    }
    //se asume que el proceso ya fue creado samente, solo resta agregarlo a la cola de scheduling
    if(list!=NULL){
        if(list->firstProcess==NULL){   //caso lista vacia
            list->firstProcess = process;
            process->next = NULL;
            list->last = process;            
            list->size = 1;
        }
        else{
            list->last->next = process;     //no cambio el current, de eso se encarga la funcion nextProcess
            process->next = NULL;
            list->last = process;
            list->size++;
        }
        scheduler->size++;
    }
    return;   
}


void scheduler_add(pid_t pid, int priority, ProcessNode * node){
    if(pid<=0 || priority < 0 || priority > 4){
        return;
    }
    if(scheduler!=NULL){
        listInsert(scheduler->list[priority], node);  
    }
    if (priority > 0) scheduler->runnableProcs++;
}



void initializeSleepingTable(void){
    int size = sizeof(SleepingTable);
    // if(size%8!=0){size+=8-(size%8);}
    sleepingTable = (SleepingTable*)my_malloc(size);
    sleepingTable->size = 0;
    sleepingTable->first = NULL;
    sleepingTable->last = NULL;
}


int sleepingTableAppend(SleepingProcess * process){ 
    
    if(sleepingTable->size+1 >= MAX_PROCESS_COUNT || process==NULL){
        return 1;
    }

    // aca solo entra cuando esta vacia
    if (sleepingTable->first == NULL){
        sleepingTable->first = process;
        sleepingTable->last = process;
    }

    // aca le meto si ya hay algo 
    else{
        sleepingTable->last->next = process;
        sleepingTable->last = process;
    }

    sleepingTable->size++;
    return 0;
}

void createSleeper(unsigned long until_ticks, int* timer_lock){
    SleepingProcess* newProc = (SleepingProcess *)my_malloc(sizeof(SleepingProcess));

    // si no tengo lugar para mas procesos lo libero
    if (sleepingTableAppend(newProc)){
        my_free(newProc);
    }
    newProc->until_ticks = until_ticks;
    newProc->pid = pcb->runningPid;
    pcb->processes[pcb->runningPid]->state = BLOCKED;
    scheduler->runnableProcs--;
    
    // esto es para que se frene hasta que haya un timer tick
    while(*timer_lock){
        _hlt();
    }

}

void delete_sleeper(pid_t pid){
    sleepingTable->first = remove_sleeper(sleepingTable->first, pid);
}

SleepingProcess * remove_sleeper(SleepingProcess * current, pid_t pid){
    if(current == NULL){
        return NULL;     //no se encontro el
    }
    if(current->pid == pid){
        SleepingProcess * toRet = current->next;
        my_free(current);
        scheduler->runnableProcs++;
        return toRet;
    }
    current->next = remove_sleeper(current->next, pid);
    return current;
}

void check_sleepers(unsigned long current_tick){

    SleepingProcess* current_proc = sleepingTable->first;
    SleepingProcess* previous_proc = NULL;

    while(current_proc != NULL){
        if (current_proc->until_ticks <= current_tick){
            pcb->processes[current_proc->pid]->state = READY;
            scheduler->runnableProcs++;

            // aca llego si ya tuve alguna iteracion 
            if (previous_proc != NULL){
                previous_proc->next = current_proc->next;

                // me aseguro de cambiar el last aca 
                if (current_proc->next == NULL){
                    sleepingTable->last = previous_proc;
                }
            }

            // Solo si es el primer caso
            else{
                sleepingTable->first = current_proc->next; 
            }

            current_proc = current_proc->next;
            my_free(current_proc);
        }

        else{
            previous_proc = current_proc;
            current_proc = current_proc->next;
        }
    }
}

void block_process(pid_t pid){
    if(pid<1 || pid>=MAX_PROCESS_COUNT){
        return;
    }
    pcb->processes[pid]->state = BLOCKED;
    scheduler->runnableProcs--;
    _force_schedule();
}

void unblock_process(pid_t pid){
    if(pid<1 || pid>=MAX_PROCESS_COUNT){
        return;
    }
    pcb->processes[pid]->state = READY;
    scheduler->runnableProcs++;
    _force_schedule();
}
//########################--CEMENTERIO DE PROCESOS--#################################################



void kill(pid_t pid){         
    _cli();
    pid_t fatherPid = pcb->processes[pid]->fatherPid;

    if(fatherPid>0){                                        
        if(pcb->processes[fatherPid]->waiting_for<-1){
            pcb->processes[fatherPid]->waiting_for++;
        }
        if(pcb->processes[fatherPid]->waiting_for==pid || pcb->processes[fatherPid]->waiting_for==-1){
            pcb->processes[fatherPid]->waiting_for=0;
            unblock_process(fatherPid);
        }
    }
    if(fatherPid!=-1){     //borro entry en array de children de padre
        boolean found = FALSE;
        for(int i=0; i<MAX_CHILDREN_COUNT && found==FALSE; i++){    //busqueda lineal
            if(pcb->processes[fatherPid]->children[i]==pid){
                pcb->processes[fatherPid]->children[i]=0;
                pcb->processes[fatherPid]->children_amount--;
                found = TRUE;
            }
        }
    }
    delete_pid_from_sems(pid);
    unschedule(pid);        //primero borro del sched porque uso la referencia a la pcb
    delete_sleeper(pid);    
    delete_from_pcb(pid);  //recien aca puedo borrar pcb
    delete_from_foreground(pid);

    if (pcb->runningPid == pid) pcb->runningPid = -1;

    _force_schedule();

    //############TODO################## 
    //cerrar pipes involucrados
    
}

void unschedule(pid_t pid){
    int processPriority = pcb->processes[pid]->priority;
   scheduler->list[processPriority]->firstProcess = delete_from_sched(scheduler->list[processPriority]->firstProcess, pid);
}
ProcessNode * delete_from_sched(ProcessNode * current, pid_t pid){
    if(current == NULL){
        return NULL;
    }
    else if(current->pid == pid){
        ProcessNode * toReturn = current->next;
        my_free(current);
        scheduler->size--;
        scheduler->list[pcb->processes[pid]->priority]->size--;
        scheduler->runnableProcs--;             
        return toReturn;
    }
    else{
        current->next = delete_from_sched(current->next, pid);
        if (current->next == NULL) scheduler->list[pcb->processes[current->pid]->priority]->last = current;
        return current;
    }
}

void delete_from_pcb(pid_t pid){    
    free_argv(pcb->processes[pid]->argc, pcb->processes[pid]->argv);
    my_free(pcb->processes[pid]);
    pcb->processes[pid] = NULL;    //new_pid chequea NULL asi que dejo los vacios en null
    pcb->size--;
}

void exit_process(){
    _cli();
    kill(get_pid());
}

int wait_pid(pid_t childPid){
    if(childPid!=-1){
        boolean found = FALSE;
        for(int i=0; i<MAX_CHILDREN_COUNT && found==FALSE; i++){
            if(pcb->processes[pcb->runningPid]->children[i]==childPid) found = TRUE;
        }
        if(found==FALSE) return -1;
    }
    pcb->processes[pcb->runningPid]->waiting_for=childPid;
    block_process(get_pid());
    return 0;
}

int wait_any_pid(){
    return wait_pid(-1);
}

int wait_all_pid(){
    return wait_pid(((pcb->processes[pcb->runningPid]->children_amount)*(-1))-1);
}

int dup(pid_t pid, uint16_t from, uint16_t to){
    if (pcb->processes[pid] == NULL) return -1;

    if (from == 0){
        pcb->processes[pid]->stdin_fd = to;
        return 0;
    }
    
    if (from == 1){
        pcb->processes[pid]->stdout_fd = to;
        return 0;
    }

    return -1;
}

uint64_t get_fd(int type){
    // stdin == 0
    if (type == 0) return pcb->processes[pcb->runningPid]->stdin_fd;
    // stdout == 1
    if (type == 1) return pcb->processes[pcb->runningPid]->stdout_fd;
    return -1;
}


// Foreground methods
int add_foreground(pid_t pid){
    ProcessNode* to_add = (ProcessNode*) my_malloc(sizeof(ProcessNode));
    if (!to_add) return -1;

    to_add->next = foregroudnProcess->firstProcess;
    to_add->pid = pid;
    foregroudnProcess->firstProcess = to_add;
    foregroudnProcess->size++;
    return 0;
}


int get_foreground(){
    if (!foregroudnProcess->size) return -1;
    return foregroudnProcess->firstProcess->pid;
}

int delete_from_foreground(int pid){
    if (!foregroudnProcess->size) return 0;

    ProcessNode* current = (ProcessNode *) my_malloc(sizeof(ProcessNode));
    if (!current) return -1;

    current->next = foregroudnProcess->firstProcess;
    int found = 0;
    
    while(current->next && !found){
        if (current->next->pid == pid){
            ProcessNode* toFree = current->next;
            current->next = toFree->next;

            if (foregroudnProcess->firstProcess->pid == pid) foregroudnProcess->firstProcess = current->next;
            foregroudnProcess->size--;

            my_free(toFree);
            found = 1;
        }
        else{
            current = current->next;
        }
    }

    return 0;
}

//##################################################################################
//the forsaken zone

/*

void priorityUp(uint32_t pid){
    Process * process = getProcess(pid);
    process->priority++;
}

void priorityDown(uint32_t pid){
    Process * process = getProcess(pid);
    process->priority--;
}

Process * getProcess(uint32_t pid){
    //aca adentro capaz tenga que recorrer la cola de procesos
    //o la tabla de procesos, segun lo que tenga
}*/


