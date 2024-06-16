#ifndef SYSCALL_HANDLER_H
#define SYSCALL_HANDLER_H


#include <stdint.h>
#include <processManager.h>
#include <sem.h>

typedef struct ProcessView{
    unsigned int memory_size;
    pid_t pid;
    int priority;
    State state;
    Registers registers;
    boolean foreground;
    pid_t fatherPid;
    int children_amount;
    char * name;
}ProcessView;

typedef struct CreateArguments{
    int priority;
    boolean orphan;
    boolean foreground;
    uint16_t stdin;
    uint16_t stdout; 
}CreateArguments;


void syscall_handler();
void sys_write(uint64_t fd, uint64_t message, uint64_t length);
int sys_read(uint64_t fd, uint64_t buffer, uint64_t length);

void sys_nanosleep(uint64_t nanos);
void sys_draw(uint64_t bitmap, uint64_t hexColor, uint64_t height, uint64_t init_x, uint64_t init_y);
void sys_screenData(uint64_t screenHeight, uint64_t screenWidth, uint64_t fontSize, uint64_t drawSize);

int read_chars(int fd, char* buffer, int length);
void getScreenData(uint16_t * screenHeight, uint16_t * screenWidth, uint8_t * fontSize, uint8_t * drawSize);

void sys_sleep(uint32_t cant, uint32_t unidad);

void sys_gettimeofday(uint64_t hrs, uint64_t min, uint64_t seg);

int sys_registers(uint64_t regs);

void *sys_malloc(uint32_t size);
void sys_free(uint64_t addr_to_free);
void sys_memState(uint64_t states);

void sys_speak(uint64_t frequence, uint64_t duration);

void sys_changeSize(uint8_t newSize, uint8_t fd);

ProcessView ** sys_get_processes(uint64_t proc_amount);
int sys_create_process(uint64_t function, uint64_t argc, uint64_t argv);
int sys_create_shiny_process(uint64_t function, uint64_t argc, uint64_t argv, uint64_t args);
int sys_waitpid(uint64_t pid);
void sys_exit(void);
void sys_kill(uint64_t pid);
void sys_change_proc_priority(uint64_t pid, uint64_t priority);
void sys_block_proc(uint64_t pid);
pid_t sys_get_pid();
void sys_yield();

sem_t * sys_sem_open(uint64_t name, uint64_t value);
int sys_sem_close(uint64_t sem);
int sys_sem_up(uint64_t sem);
int sys_sem_down(uint64_t sem);

int sys_pipe(uint64_t pipefd);
int sys_pclose(uint64_t fd);
int sys_peek_read_pipe(uint64_t fd);
int sys_peek_pipe(uint64_t fd);


#endif
