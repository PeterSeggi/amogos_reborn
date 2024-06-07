#ifndef PROCESSTEST_H
#define PROCESSTEST_H

#include <stdint.h>

#define INITIAL_PROCESS_SIZE 2000
#define MAX_PROCESS_COUNT 100
#define MAX_CHILDREN_COUNT 50
#define DEFAULT_PRIORITY 4

typedef enum boolean{
    FALSE,
    TRUE
}boolean;

typedef enum State{
    READY,
    RUNNING,
    BLOCKED
}State;

typedef int pid_t;

typedef struct Registers{
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rip;
}Registers;

typedef struct Process{
    unsigned int memory_size;
    pid_t pid;
    int priority;
    struct Process * children[MAX_PROCESS_COUNT];
    State state;
    Registers registers;
    boolean foreground;
    pid_t fatherPid;
}Process;

typedef struct ProcessTable{
    Process * processes[MAX_PROCESS_COUNT];
    unsigned int size;
    pid_t runningPid;
}ProcessTable;

typedef struct ProcessNode{
    pid_t pid;
    struct ProcessNode * next;
}ProcessNode;

typedef struct ProcessList{
    ProcessNode * firstProcess;
    ProcessNode * last;
    ProcessNode * current;
    int size;
}ProcessList;

typedef struct PriorityArray{
    ProcessList * list[5];
    int size;
    int priority[10];
    int currentPriorityOffset;
    int runnableProcs;
}PriorityArray;

typedef struct SleepingProcess{
    pid_t pid;
    unsigned long until_ticks;
    struct SleepingProcess* next; 
}SleepingProcess;

typedef struct SleepingTable{
    unsigned int size;
    SleepingProcess * first;
    SleepingProcess * last;
}SleepingTable;




Process * create_process(void * function);
Process * create_shiny_process(void * function, unsigned int priority, boolean orphan);
boolean add_child(int pid, Process * child);
int nextProcess(void);
void scheduler_add(pid_t pid, int priority, ProcessNode * node);
void initializeScheduler(void);
int pcb_append(Process * process);
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
void initialize_pcb(void);
void initializeScheduler(void);
void _cli();
void _sti();
void _hlt();
void _setUser(void);
void initializeSleepingTable(void);
int sleepingTableAppend(SleepingProcess * process);
int createSleeper(unsigned long until_ticks, int* timer_lock);
int check_sleepers(unsigned long current_tick);

// queda comentada porq es quasi-privada
// void * schedule(void * rsp);

int get_pcb_size();
Process ** get_processes();
pid_t get_pid();

#endif
