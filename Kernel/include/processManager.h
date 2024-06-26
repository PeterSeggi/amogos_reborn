#ifndef PROCESSTEST_H
#define PROCESSTEST_H

#include <stdint.h>

#define INITIAL_PROCESS_SIZE 8192
#define MAX_PROCESS_COUNT 100
#define MAX_CHILDREN_COUNT 50
#define MAX_PROCESS_NAME 20
#define DEFAULT_PRIORITY 4

#define KEY_FD 4
#define VID_FD 7

typedef enum boolean{
    FALSE,
    TRUE
}boolean;

typedef int pid_t;

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
    pid_t fatherPid;
    pid_t pid;
    int priority;
    State state;
    Registers registers;
    boolean foreground;
    pid_t children[MAX_CHILDREN_COUNT];
    char* name;
    int children_amount;
    pid_t waiting_for;
    uint16_t stdin_fd;
    uint16_t stdout_fd;
    int argc;
    char ** argv;
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


Process * create_process(void * function, int argc, char ** argv);
Process * create_shiny_process(void * function, int argc, char ** argv, int priority, boolean orphan, boolean foreground, uint16_t stdin, uint16_t stdout);

void scheduler_add(pid_t pid, int priority, ProcessNode * node);
pid_t nextProcess(void);
int pcb_append(Process * process);
//void destroyProcess(Process * process);
void stackTest(int myrsp);
void createStack(void);
void stackPrep(void);
void stackUnprep(void);
uint64_t initializeStack(void * rsp, void * rip, int argc, char ** argv);
void initialize_pcb(void);
void initializeScheduler(void);
void _cli();
void _sti();
void _hlt();
void _setUser(void);
void initializeSleepingTable(void);
int sleepingTableAppend(SleepingProcess * process);
void create_sleeper(unsigned long until_ticks);
void check_sleepers(unsigned long current_tick);

// queda comentada porq es quasi-privada
// void * schedule(void * rsp);

int get_pcb_size();
Process ** get_processes();
pid_t get_pid();
void block_process(pid_t pid);
void silent_block(pid_t pid);
void silent_unblock(pid_t pid);
void unblock_process(pid_t pid);

boolean check_valid_pid(pid_t pid);
State get_pid_state(pid_t pid);
void nice(pid_t pid, int priority);

void exit_process();
void kill(pid_t pid);
void kill_children(pid_t pid);
void yield();

int wait_pid(pid_t childPid);
int wait_any_pid();
int wait_all_pid();

int dup(pid_t pid, uint16_t from, uint16_t to);
uint64_t get_fd(int type);

int add_foreground(pid_t pid);
int get_foreground();
int get_foreground_fd();
int delete_from_foreground(int pid);
int pid_in_foreground(pid_t pid);

int getRunningProceses();

#endif
