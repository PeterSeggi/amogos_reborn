#include <keyboard.h>
#include <lib.h>
#include <stdint.h>
#include <syscall_handler.h>
#include <videoDriver.h>
#include <time.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define ERRCOLORFONT 0xFF0000 // texto rojo, bg gris;
#define ERRCOLORBACK 0xDADADA // texto rojo, bg gris;

void syscall_handler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax) {
  switch (rax) {
  case (0x01):
    sys_write(rdi, rsi, rdx);
    break;

  case (0x00):
    sys_read(rdi, rsi, rdx);
    break;
    
  case (0x23):
    sys_sleep(rdi, rsi);
    break;  

  case (0x4e):
    sys_gettimeofday(rdi, rsi, rdx);
    break;
  }
}

void sys_write(uint64_t fd, uint64_t message, uint64_t length) {
  switch (fd) {
  case (STDOUT):
    printCant((uint8_t *)message, length);
    break;

  case (STDERR):
    printColorCant((uint8_t *)message, length, ERRCOLORFONT, ERRCOLORBACK);
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

void sys_sleep(uint32_t  cant,uint32_t  unidad){
  newLine();
  print("antes de sleep");
  newLine();
  sleep(cant, unidad); 
  print("despues de sleep");
  newLine();
}

void sys_gettimeofday(int *hrs, int *min, int *seg){
  printTime(hrs, min, seg);
 }