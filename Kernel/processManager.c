#include <stdint.h>
#include <mman.h>
#include <processManager.h>






//variables globales
ProcessTable * processTable = NULL;
PriorityArray * scheduler = NULL;
SleepingTable * sleepingTable = NULL;

int schedule_lock = 1;

void testNext(){
    int firstProcess = nextProcess();
    int secondProcess = nextProcess();
    int firstagain = nextProcess();
    int secondagain = nextProcess();
    int firstoncenoceagain = nextProcess();
    int secondoncenoceagain = nextProcess();
}


void cosa11(){
    int cosa = 1;
    cosa++;    
}
void cosa12(){
    int cosa = 12;
}

void cosa21(){
    int cosa = 21;
}

void cosa22(){
    int cosa = 22;
}
void my_main(void){
    int message = 1000;
    createProcess(&cosa11);
    int message2 = 110;
    createProcess(&cosa12);
    int message3 = 120;
}


//funciones publicas 
void initializeProcessTable(void){
    int size = sizeof(ProcessTable);
    if(size%8!=0){size+=8-(size%8);}
    processTable = (ProcessTable *)my_malloc(size);
    processTable->size = 0;
    processTable->runningPid = 0;
}

void executeProcess(uint64_t rip){
    ((void(*)())rip)();
}

void initializeScheduler(){
    int size = sizeof(PriorityArray);
    scheduler = my_malloc(size);
    scheduler->size = 0;
    scheduler->currentPriorityOffset = 0;    //señalizo que no tiene procesos corriendo aun
    int myPriorities[10] = {4, 4, 4, 4, 3, 3, 3, 2, 2, 1};
    for (int i = 0; i < 10; i++) {
        scheduler->priority[i] = myPriorities[i];
    }
    for(int i=0; i<5; i++){
        scheduler->list[i] = my_malloc(sizeof(ProcessTable));

        if(scheduler->list[i]!=NULL){
            scheduler->list[i]->firstProcess=NULL;
            scheduler->list[i]->last=NULL;
            scheduler->list[i]->current=NULL;
            scheduler->list[i]->size=0;
        }
    }
    //createProcess(&my_main);   
    createProcessWithpriority(&_idle, 0);     //proceso vigilante _hlt
    createProcess(0x400000);

    schedule_lock = 0;
    _idle();
}

Process * createProcess(void * function){
    return createProcessWithpriority(function, DEFAULT_PRIORITY);  //por defecto se crea con prioridad 4
}

int createProcessWithpriority(void * function, unsigned int priority){
    _cli();
    Process * process = my_malloc(INITIAL_PROCESS_SIZE);
    process->memory_size = INITIAL_PROCESS_SIZE;
    process->state = READY;  

    //inicializa el stack
    process->registers.rbp = ( (uint64_t)process + INITIAL_PROCESS_SIZE ); 
    process->registers.rsp = process->registers.rbp;        //inicialmente stack vacio
    process->registers.rip = (uint64_t)function;  //direccion de la funcion a ejecutar
    process->registers.rsp = initializeStack(process->registers.rsp, process->registers.rip); 
    process->pid = processTable->size + 1;       //cada nuevo proceso recibe el pid siguiente en orden natural
    
    int result = processTableAppend(process);
    if(result == 1){    //error en la creacion del proceso
        //free(process);  //libero recursos utlizados
        _sti();
        return NULL;
    }

    int size = sizeof(ProcessNode);
    if(size%8!=0){size+=8-(size%8);}
    ProcessNode * node = my_malloc(size);
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
    }
    addProcess(process->pid, priority, node);  //agrega el proceso a la cola de scheduling con la prioridad deseada
    _sti();
    return process;                     
}

int processTableAppend(Process * process){ 
    
    if(processTable->size+1 >= MAX_PROCESS_COUNT || process==NULL){
        return 1;
    }
    processTable->processes[process->pid] = process;
    processTable->size++;
    return 0;
}

void * schedule(void * rsp){

    if (schedule_lock == 1){
        return rsp;
    } 

    int priority = scheduler->priority[scheduler->currentPriorityOffset];
    if(scheduler->list[priority]->current != NULL){ //si estoy en el primer proceso no me guardo el stack de kernel
        processTable->processes[processTable->runningPid]->registers.rsp = rsp;           
    }
    processTable->runningPid = nextProcess(); 
    return processTable->processes[processTable->runningPid]->registers.rsp;
    //return rsp;
}

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

void addProcess(int pid, int priority, ProcessNode * node){
    if(pid<=0 || priority < 0 || priority > 4){
        return;
    }
    if(scheduler!=NULL){
        listInsert(scheduler->list[priority], node);  
    }
}

int nextProcess(){
    int priority = scheduler->priority[scheduler->currentPriorityOffset];
    return nextProcessInList(scheduler->list[priority]);
}

int nextProcessInList(ProcessList * list){
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
    else if(list->current == NULL){     //primera vez poniendo un proceso de la lista en marcha
        list->current = list->firstProcess; 
        // aca tendria q hacer q si esta blocked q mande al siguiente
        return list->current->pid;
    }
    else{
        if(list->current->next == NULL){    //ultimo proceso de la lista
            list->current = NULL; //reseteo la lista
            return nextProcessInList(scheduler->list[getNextPriority()]);  //esto deja seteado
        }
        else{
            list->current = list->current->next;    //quiero retornar el pid del SIGUIENTE
            // mismo aca, si el siguiente esta blocked tendria q devolver el siguiente 
            return list->current->pid;
        }
    }
}

int getNextPriority(){
    scheduler->currentPriorityOffset = (scheduler->currentPriorityOffset+1)%10;
    return scheduler->priority[scheduler->currentPriorityOffset];
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

int createSleeper(unsigned long until_ticks){
    SleepingProcess* newProc = (SleepingProcess *)my_malloc(sizeof(SleepingProcess));

    // si no tengo lugar para mas procesos lo libero
    if (sleepingTableAppend(newProc)){
        my_free(newProc);
    }
    newProc->until_ticks = until_ticks;
    newProc->pid = processTable->runningPid;
    processTable->processes[processTable->runningPid]->state = BLOCKED;
}

int check_sleepers(unsigned long current_tick){

    SleepingProcess* current_proc = sleepingTable->first;
    SleepingProcess* previous_proc = NULL;

    while(current_proc != NULL){
        if (current_proc->until_ticks == current_tick){
            processTable->processes[current_proc->pid]->state = READY;

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


/*void destroyProcess(Process * process){
    my_free(process->memory_start);
}*/


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


