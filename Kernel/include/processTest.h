#ifndef PROCESSTEST_H
#define PROCESSTEST_H

#include <stdint.h>

#define INITIAL_PROCESS_SIZE 100
#define MAX_PROCESS_COUNT 100

typedef enum State{
    READY,
    RUNNING,
    BLOCKED,
    DEAD
}State;

typedef struct Registers{
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
    uint64_t rflags;
}Registers;


typedef struct Process{
    void * memory_start; //inicio de su memoria reservada
    unsigned int memory_size;
    unsigned int priority;
    uint32_t pid;
    State state;
    Registers registers;
}Process;



typedef struct ProcessTable{
    Process * processes[MAX_PROCESS_COUNT];
    unsigned int size;
}ProcessTable;

Process * createProcess(ProcessTable * processTable);
int processTableAppend(Process * process, ProcessTable * processTable);
//void destroyProcess(Process * process);
void stackTest(int myrsp);
void createStack(void);
void stackPrep(void);
void stackUnprep(void);
void cosa11(void);
void cosa12(void);
void cosa21(void);
void cosa22(void);
void executeProcess(Process * process);

#endif