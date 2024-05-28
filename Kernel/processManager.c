
#include <stdint.h>
#include <mman.h>
#include <processManager.h>






//variables globales
ProcessTable * processTable = NULL;
PriorityArray * scheduler = NULL;

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
    processTable->runningPid = 1;
    //todo: crear el proceso halt aca y que corra cuando no hay nada? kcyo maybe :D
}

void executeProcess(uint64_t rip){
    ((void(*)())rip)();
}

void initializeScheduler(){
    int size = sizeof(PriorityArray);
    if(size%8!=0){size+=8-(size%8);}
    scheduler = my_malloc(size);
    scheduler->size = 0;
    scheduler->currentPriority = 4;    //señalizo que no tiene procesos corriendo aun
    for(int i=0; i<5; i++){
        int listSize = sizeof(ProcessTable);
        if(listSize%8!=0){listSize+=8-(listSize%8);}
        scheduler->list[i] = my_malloc(listSize);

        if(scheduler->list[i]!=NULL){
            scheduler->list[i]->firstProcess=NULL;
            scheduler->list[i]->last=NULL;
            scheduler->list[i]->current=NULL;
            scheduler->list[i]->size=0;
        }
    }
    //createProcess(&_setUser);
    createProcess(&my_main);   //proceso vigilante _hlt
}

Process * createProcess(void * function){
    _cli();
    Process * process = my_malloc(INITIAL_PROCESS_SIZE);
    process->memory_size = INITIAL_PROCESS_SIZE;
    process->state = READY;  

    //inicializa el stack
    process->registers.rbp = ( (uint64_t)process + INITIAL_PROCESS_SIZE ); 
    process->registers.rsp = process->registers.rbp;        //inicialmente stack vacio
    process->registers.rip = (uint64_t)function;  //direccion de la funcion a ejecutar
    process->registers.rsp = initializeStack(process->registers.rsp, process->registers.rip); 
    
    if(processTable !=NULL){
        process->pid = processTable->size + 1;          //cada nuevo proceso recibe el pid siguiente en orden natural
    }
    else{
        process->pid = 1;
    }    
    int result = processTableAppend(process);
    if(result == 1){    //error en la creacion del proceso
        //free(process);  //libero recursos utlizados
        return NULL;
    }

    int size = sizeof(ProcessNode);
    if(size%8!=0){size+=8-(size%8);}
    ProcessNode * node = my_malloc(size);
    if(node == NULL){return NULL;}
    node->pid = process->pid;
    addProcess(process->pid, 4, node);  //agrega el proceso a la cola de scheduling
    return process;                     //prioridad 4 por defecto
    _sti();
}

int processTableAppend(Process * process){ 
    if(processTable->size >= MAX_PROCESS_COUNT || process==NULL){
        return 1;
    }
    processTable->processes[process->pid] = process;
    processTable->size++;
    return 0;
}

void * schedule(void * rsp){
    if(scheduler->list[scheduler->currentPriority]->current != NULL){ //si estoy en el primer proceso no me guardo el stack de kernel
        int runningPid = scheduler->list[scheduler->currentPriority]->current->pid;
        processTable->processes[runningPid]->registers.rsp = rsp;           
    }    
    processTable->runningPid = (uint64_t)nextProcess(); 
    return processTable->processes[processTable->runningPid]->registers.rsp;
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

/*void firstInsert(ProcessNode * node){
    scheduler->currentPriority = 4;     //ejecuta procesos con mas prioridad primero
    scheduler->list[4]->firstProcess = node;
    scheduler->list[4]->firstProcess->next = NULL;
    scheduler->list[4]->last = node;
    scheduler->list[4]->current = node;
    scheduler->list[4]->size = 1; 
}*/

void addProcess(int pid, int priority, ProcessNode * node){
    if(pid<=0 || priority < 0 || priority > 4){
        return;
    }
    if(scheduler!=NULL){
        listInsert(scheduler->list[priority], node);  
    }
}

int nextProcessInList(ProcessList * list){
    if(list!=NULL){
        if(list->current != NULL){
            if(list->current->next == NULL){    //ultimo proceso de esa prioridad
                list->current = list->firstProcess;    //reseteo la lista
                scheduler->currentPriority =  (scheduler->currentPriority - 1)%5;   //paso a la proxima prioridad
                return nextProcessInList(scheduler->list[scheduler->currentPriority]);
            }
            else{
                int toReturn = list->current->pid;
                list->current = list->current->next;
                return toReturn;
            }
        }
        else{   //el scheduler tiene el primer proceso cargado pero no se puso en marcha
            if(list->firstProcess != NULL){
                list->current = list->firstProcess;
                return list->current->pid;
            }
        }
    }
    scheduler->currentPriority =  (scheduler->currentPriority - 1)%5;   //paso a la proxima prioridad
    return nextProcessInList(scheduler->list[scheduler->currentPriority]);
}

int nextProcess(){
    if(scheduler!=NULL){   
        //si todas las listas de prioridades estan vacias, se crea un loop infinito   
        return nextProcessInList(scheduler->list[scheduler->currentPriority]);  
    }
    return NULL;
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


