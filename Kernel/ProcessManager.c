
 #include <stdint.h>
 #include <mman.h>
/*typedef struct {
    uint32_t pid;
    uint32_t ppid;
    uint32_t priority;
    uint32_t state;
    uint32_t time;
    uint32_t stack;
    uint32_t stack_size;
    uint32_t entry;
    uint32_t name;
} Process;*/

#define MAX_PROCESS_COUNT 100

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
    void * code_start;  //inicio de su codigo
    unsigned int memory_size;
    unsigned int priority;
    uint32_t pid;
    Registers registers;
}Process;

typedef struct ProcessTable{
    Process processes[MAX_PROCESS_COUNT];
    unsigned int size;
}ProcessTable;

void createProcess(Process * process, void * code_start, unsigned int memory_size, unsigned int priority, uint32_t pid){
    process->memory_start = my_malloc(memory_size);
    process->code_start = code_start;
    process->memory_size = memory_size;
    process->priority = priority;
    process->pid = pid;
}

void destroyProcess(Process * process){
    my_free(process->memory_start);
}

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
}
