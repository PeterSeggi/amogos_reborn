#include <stdint.h>
#include <mman.h>
#include <processManager.h>
#include <interrupts.h>


ProcessNode * delete_from_sched(ProcessNode * current, pid_t pid);
void delete_from_pcb(pid_t pid);
void destroyProcess(Process * process);
pid_t new_pid();
void unschedule(pid_t pid);
pid_t nextProcessInList(ProcessList * list);
int getNextPriority();
SleepingProcess * remove_sleeper(SleepingProcess * current, pid_t pid);
void delete_sleeper(pid_t pid);
boolean add_child(pid_t fatherPid, pid_t childPid);

//variables globales
ProcessTable * pcb = NULL;
PriorityArray * scheduler = NULL;
SleepingTable * sleepingTable = NULL;

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
    create_shiny_process(&_idle, 0, TRUE);     //proceso vigilante _hlt
    create_shiny_process((void *)0x400000, 4, TRUE);

    schedule_lock = 0;
    _idle();
}

Process * create_process(void * function){
    return create_shiny_process(function, DEFAULT_PRIORITY, FALSE);  //por defecto se crea con prioridad 4
}

Process * create_shiny_process(void * function, int priority, boolean orphan){
    _cli();
    Process * process = (Process *)my_malloc(INITIAL_PROCESS_SIZE);
    process->memory_size = INITIAL_PROCESS_SIZE;
    process->state = READY;  
    process->foreground = FALSE;
    process->children_amount = 0;
    
    //inicializa el stack
    process->registers.rbp = ( (uint64_t)process + INITIAL_PROCESS_SIZE ); 
    process->registers.rsp = process->registers.rbp;        //inicialmente stack vacio
    process->registers.rip = (uint64_t)function;  //direccion de la funcion a ejecutar
    process->registers.rsp = initializeStack((void *)process->registers.rsp, (void *)process->registers.rip); 
    process->pid = new_pid();       //cada nuevo proceso recibe el pid siguiente en orden natural
    
    if(process->pid == -1){
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
        //free(process);  //libero recursos utlizados
        _sti();
        return NULL;
    }

    int size = sizeof(ProcessNode);
    if(size%8!=0){size+=8-(size%8);}
    ProcessNode * node = (ProcessNode *)my_malloc(size);
    if(node == NULL){
        //free(process);
        _sti();
        return NULL;
    }
    node->pid = process->pid;
    if(priority>4){
        //free(process);
        //free(node);
        _sti();
        return NULL;
    }
    scheduler_add(process->pid, priority, node);  //agrega el proceso a la cola de scheduling con la prioridad deseada
    
    _sti();
    return process;                     
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


//###############################--SCHEDULING ZONE--#########################################################

void * schedule(void * rsp){

    if (schedule_lock == 1){
        return rsp;
    } 

    int priority = scheduler->priority[scheduler->currentPriorityOffset];
    if(scheduler->list[priority]->current != NULL){ //si estoy en el primer proceso no me guardo el stack de kernel
        pcb->processes[pcb->runningPid]->registers.rsp = (uint64_t)rsp;           
    }
    pcb->runningPid = nextProcess();   //next process ignora los procesos bloqueados
    return (void *)pcb->processes[pcb->runningPid]->registers.rsp;
    //return rsp;
}


pid_t nextProcess(){
    int priority = scheduler->priority[scheduler->currentPriorityOffset];
    return nextProcessInList(scheduler->list[priority]);
}


pid_t nextProcessInList(ProcessList * list){

    // si no tengo nada q correr voy directo a idle
    if(scheduler->runnableProcs == 0){
        // basically el pid del idle
        return scheduler->list[0]->firstProcess->pid;
    }

    if(list->size == 0){            //si la prioridad no tiene procesos, sigue buscando
        /**
         * int priority = scheduler->priority[scheduler->currentPriorityOffset];
         * int newPrior;
         * while((newPrior=getNextPriority()) == priority){
         *      skippeo todos los 333 si es que en la lista 3 no hay ningun proceso
         * }
        */
        return nextProcessInList(scheduler->list[getNextPriority()]);
    }
    else if(list->current == NULL){     //primera vez poniendo un proceso de la lista en marcha || reseteo lista
        list->current = list->firstProcess; 
        if (pcb->processes[list->current->pid]->state == BLOCKED){
            return nextProcessInList(list); //el primero estaba bloqueado, le paso la misma lista para que tome al siguiente 
        }
        else{
            return list->current->pid;
        }
    }
    else{
        if(list->current->next == NULL){    //ultimo proceso de la lista
            list->current = NULL; //reseteo la lista
            return nextProcessInList(scheduler->list[getNextPriority()]);  //esto deja seteado
        }
        else{
            list->current = list->current->next;    //quiero retornar el pid del SIGUIENTE
            if (pcb->processes[list->current->pid]->state == BLOCKED){
                return nextProcessInList(list); //paso al siguiente de la lista
            }
            else{
                return list->current->pid;
            }
        }
    }
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
    //se asume que el proceso ya fue creado exitosamente, solo resta agregarlo a la cola de scheduling
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
    pcb->processes[pcb->runningPid]->state = BLOCKED;
    scheduler->runnableProcs--;
}

void unblock_process(pid_t pid){
    if(pid<1 || pid>=MAX_PROCESS_COUNT){
        return;
    }
    pcb->processes[pcb->runningPid]->state = READY;
    scheduler->runnableProcs++;
}
//########################--CEMENTERIO DE PROCESOS--#################################################



void kill(pid_t pid){     /*ALERT: en caso de que se borre el q esta corriendo, schedule lo va a sacar en proximo loop*/
    
    if(pcb->processes[pid]->fatherPid!=-1){     //borro entry en array de children de padre
        pid_t fatherPid = pcb->processes[pid]->fatherPid;
        pcb->processes[fatherPid]->children[pid] = 0;
    }
    unschedule(pid);        //primero borro del sched porque uso la referencia a la pcb
    delete_sleeper(pid);
    //TODO delete from semaphores
    delete_from_pcb(pid);  //recien aca puedo borrar pcb
    
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
        scheduler->runnableProcs--;             
        return toReturn;
    }
    else{
        current->next = delete_from_sched(current->next, pid);
        return current;
    }
}

void delete_from_pcb(pid_t pid){
    my_free(pcb->processes[pid]);
    pcb->processes[pid] = NULL;    //new_pid chequea NULL asi que dejo los vacios en null
    pcb->size--;
}

void exit_process(){
    pid_t fatherPid = pcb->processes[get_pid()]->fatherPid;
    if(pcb->processes[fatherPid]->waiting_for<-1){
        pcb->processes[fatherPid]->waiting_for++;
    }
    if(pcb->processes[fatherPid]->waiting_for==get_pid() || pcb->processes[fatherPid]->waiting_for==-1){
        pcb->processes[fatherPid]->waiting_for=0;
        boolean found = FALSE;
        pid_t childPid = get_pid();
        for(int i=0; i<MAX_CHILDREN_COUNT && found==FALSE; i++){
            if(pcb->processes[pcb->runningPid]->children[i]==childPid){
                pcb->processes[pcb->runningPid]->children[i]=0;
                found = TRUE;
            }
        }
        unblock_process(fatherPid);
    }
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


