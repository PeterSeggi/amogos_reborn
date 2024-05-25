
#include <stdint.h>
#include <mman.h>
#include "processTest.h"




/*funciones*/


//definidas globalmente porque son comunes a todos los procesos



Process * createProcess(ProcessTable * processTable){
    Process * process = my_malloc(INITIAL_PROCESS_SIZE);
    //ahora hay que inicializar el stack

    unsigned int maxStackSize = INITIAL_PROCESS_SIZE/3;     //tamaño del stack
    process->registers.rbp = ( (uint64_t)process + INITIAL_PROCESS_SIZE ); 
    process->registers.rsp = process->registers.rbp;        //inicialmente stack vacio
    
    process->registers.rip = (uint64_t)&cosa11;  //direccion de la funcion a ejecutar
    
    createStack();


    //process->priority = 0;                              //prioridad inicial 0, debera ser asignada localmente
    //State state = READY;    ???????? esto no tiene sentido

    if(processTable !=NULL){
        process->pid = processTable->size + 1;          //cada nuevo proceso recibe el pid siguiente en orden natural
    }
    else{
        process->pid = 1;
    }    
    int result = processTableAppend(process, processTable);
    //if(result == 1){    //error en la creacion del proceso
    //    free(process);  //libero recursos utlizados
    //    return NULL;
    //}
    return process;
}

int processTableAppend(Process * process, ProcessTable * processTable){ 
    //esta garantizado que processTable no es NULL porque se crea mas arriba
    if(processTable->size >= MAX_PROCESS_COUNT){    //error, no sepueden crear mas procesos
        return 1;
    }
    processTable->processes[processTable->size] = process;
    processTable->size++;
    return 0;
}

void executeProcess(Process * process){
    if(process!=NULL){
        ((void (*)())process->registers.rip)();        
    }
}



void cosa11(){
    int cosa = 11;
    stackPrep();
    int saracatunga = 777;    
}



void cosa12(){
    int cosa = 12;
    stackUnprep();
}

void cosa21(){
    int cosa = 21;
}

void cosa22(){
    int cosa = 22;
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


