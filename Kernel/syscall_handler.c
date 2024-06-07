#include <keyboard.h>
#include <lib.h>
#include <stdint.h>
#include <syscall_handler.h>
#include <videoDriver.h>
#include <time.h>
#include <registers.h>
#include <sound.h>
#include <mman.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

void failure_free(ProcessView ** ptr_list, int size);
ProcessView ** set_processes(uint16_t * proc_amount);

void syscall_handler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax) {
  switch (rax) {

  case (0x00):
    sys_read(rdi, rsi, rdx);
    break;

  case (0x01):
    sys_write(rdi, rsi, rdx);
    break;

  case (0x4e):
    sys_gettimeofday(rdi, rsi, rdx);
    break;
    
  case (0x23):
    sys_sleep(rdi, rsi);
    break;  

  case (0x77):
    sys_draw(rdi, rsi, rdx, rcx, r8);
    break;

  case (0x78):
    sys_registers(rdi);
    break;

  case (0x83):
    sys_screenData(rdi, rsi, rdx, rcx);
    break;

  case (0x84):
    sys_speak(rdi, rsi);
    break;

  case (0x86):
    sys_malloc(rdi);
    break;

  case (0x87):
    sys_free(rdi);
    break;

  case (0x88):
    sys_memState(rdi);
    break;

  case (0x93):
    sys_changeSize(rdi, rsi);
    break;

  case (0xA0):
    sys_get_processes(rdi);
    break;

  case (0xA1):
    sys_create_process(rdi, rsi, rdx);
    break;
  }
}

void sys_write(uint64_t fd, uint64_t message, uint64_t length) {
  switch (fd) {
      case (STDOUT):
        printCant((char *)message, length);
        break;

      case (STDERR):
        printColorCant((char *)message, length, ERRCOLORFONT, ERRCOLORBACK);
        break;
      }
}

int sys_read(uint64_t fd, uint64_t buffer, uint64_t length) {
  int retVal = 0;
  retVal = read_chars(fd, (char *)buffer, length);
  return retVal;
}

int read_chars(int fd, char *buffer, int length) {
  int chars_read = 0;
  for (int i = 0; i < length; i++) {
    chars_read++;
    buffer[i] = read_key(fd);
    if (buffer[i] == 0) {
      i = length; // si llego a un null dejo de leer
      chars_read--;
    }
  }
  return chars_read;
}

void sys_draw(uint64_t bitmap, uint64_t hexColor, uint64_t height, uint64_t init_x, uint64_t init_y){
  printBitmap((uint16_t *) bitmap, hexColor, height, init_x, init_y);
}

void sys_screenData(uint64_t screenHeight, uint64_t screenWidth, uint64_t fontSize, uint64_t drawSize){
  getScreenData((uint16_t *) screenHeight, (uint16_t *) screenWidth, (uint8_t *) fontSize, (uint8_t *) drawSize);
}

void getScreenData(uint16_t * screenHeight, uint16_t * screenWidth, uint8_t * fontSize, uint8_t * drawSize){
  *screenHeight=getScreenHeight();
  *screenWidth=getScreenWidth();
  *fontSize=getFontSize();
  *drawSize=getDrawSize();
}

void sys_sleep(uint32_t  cant,uint32_t  unidad){
  sleep(cant, unidad); 
}

void sys_gettimeofday(uint64_t hrs, uint64_t min, uint64_t seg){
  printTime((int*) hrs, (int*)min, (int *)seg);
 }

int sys_registers(uint64_t regs){
    return getRegs((uint64_t*) regs);
}

void sys_changeSize(uint8_t newSize, uint8_t fd){
  switch(fd){
    case(1):
      changeFontSize(newSize);
      break;

    case(2):
      changeDrawSize(newSize);
      break;
      
    default:
      break;
  }
}

void sys_speak(uint64_t frequence, uint64_t duration){
    beep((uint32_t) frequence, (int) duration);
}

void *sys_malloc(uint16_t size){
  return my_malloc(size);
}

void sys_free(uint64_t addr_to_free){
  return my_free((void *)addr_to_free);
}

void set_memstates(uint64_t * states){
  *(states) = get_mem_total();
  *(states+1)  = get_mem_vacant();
  *(states+2) = get_mem_occupied();
}

void sys_memState(uint64_t states){
    set_memstates((uint64_t *) states);
}

//returns -1 when my_malloc fails
ProcessView ** sys_get_processes(uint64_t proc_amount){
  return set_processes((uint16_t *) proc_amount);
  
}

ProcessView ** set_processes(uint16_t * proc_amount){
  if(!get_pcb_size()) return 0;
  Process ** processes = get_processes();
  int process_amount = get_pcb_size();
  ProcessView ** to_ret = (ProcessView **) my_malloc(sizeof(ProcessView *)*process_amount);
  if(!to_ret) return NULL;
  //from index=1 since pid0 is not valid
  for(int i = 1, copied=0; i<MAX_PROCESS_COUNT && copied<process_amount; i++){
    if(processes[i]){//only copies if process by that pid exists
      to_ret[copied] = (Process *) my_malloc(sizeof(ProcessView));
      if(!to_ret[copied]){
        failure_free(to_ret, copied-1);
        my_free(to_ret);
        return NULL;
      }
      to_ret[copied]->memory_size=processes[i]->memory_size;
      to_ret[copied]->pid=processes[i]->pid;
      to_ret[copied]->priority=processes[i]->priority;
      to_ret[copied]->state=processes[i]->state;
      to_ret[copied]->registers=processes[i]->registers;
      to_ret[copied]->foreground=processes[i]->foreground;
      if(processes[i]->fatherPid==-1) to_ret[copied]->fatherPid=0;//no father
      else to_ret[copied]->fatherPid=processes[i]->fatherPid;
      to_ret[copied]->children_amount=processes[i]->children_amount;
      copied++;
    }
  }
  *proc_amount = process_amount;
  return to_ret;
}

void failure_free(ProcessView ** ptr_list, int size){
  while(size>=0){
    my_free(ptr_list[size--]);
  }
}

void sys_create_process(uint64_t function, uint64_t priority, uint64_t orphan){
  int check_case = (int) priority;
  if(check_case<0) create_process((void *) function);
  else create_shiny_process((void *) function, (int) priority, (boolean) orphan);
}