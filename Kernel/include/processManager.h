#ifndef PROCESSTEST_H
#define PROCESSTEST_H

#include <stdint.h>

#define INITIAL_PROCESS_SIZE 1000
#define MAX_PROCESS_COUNT 100
#define DEFAULT_PRIORITY 4

typedef enum State{
    READY,
    RUNNING,
    BLOCKED,
    DEAD
}State;

typedef struct Registers{
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rip;
}Registers;

typedef struct Process{
    void * memory_start; //inicio de su memoria reservada
    unsigned int memory_size;
    uint32_t pid;
    State state;
    Registers registers;
}Process;

typedef struct ProcessTable{
    Process * processes[MAX_PROCESS_COUNT];
    unsigned int size;
    uint16_t runningPid;
}ProcessTable;

typedef struct ProcessNode{
    int pid;
    struct ProcessNode * next;
}ProcessNode;

typedef struct ProcessList{
    ProcessNode * firstProcess;
    ProcessNode * last;
    ProcessNode * current;
    int size;
}ProcessList;

typedef struct PriorityArray{
    // esto no tendrian que ser 4 porq solo tenemos hasta esa prioridad y no hay priori 0?
    ProcessList * list[5];
    int size;
    int priority[10];
    int currentPriorityOffset;
}PriorityArray;

typedef struct SleepingProcess{
    int pid;
    unsigned long until_ticks;
    struct SleepingProcess* next; 
}SleepingProcess;

typedef struct SleepingTable{
    unsigned int size;
    SleepingProcess * first;
    SleepingProcess * last;
}SleepingTable;

Process * createProcess(void * function);
int nextProcess(void);
void addProcess(int pid, int priority, ProcessNode * node);
void initializeScheduler(void);
int processTableAppend(Process * process);
//void destroyProcess(Process * process);
void stackTest(int myrsp);
void createStack(void);
void stackPrep(void);
void stackUnprep(void);
void cosa11(void);
void cosa12(void);
void cosa21(void);
void cosa22(void);
uint64_t initializeStack(void * rsp, void * rip);
void initializeProcessTable(void);
void initializeScheduler(void);
int createProcessWithpriority(void * function, unsigned int priority);
void _cli();
void _sti();
void _hlt();
void _setUser(void);
void initializeSleepingTable(void);
int sleepingTableAppend(SleepingProcess * process);
int createSleeper(unsigned long until_ticks);
int check_sleepers(unsigned long current_tick);
void * schedule(void * rsp);

#endif
