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
    
    case (0x00):
      sys_read(rdi, rsi, rdx);
      break;

    case (0x01):
      sys_write(rdi, rsi, rdx);
      break;

    case (0x23):
      sys_sleep(rdi, rsi);
      break;  
  /*
  case (0x4e):
   sys_gettimeofday(rdi, rsi);
   break;
  */}
}

void sys_write(uint64_t fd, uint64_t message, uint64_t length) {
  switch (fd) {
  case (STDOUT):
    printCant((uint8_t *)message, length);
    ncNewline();
    break;

  case (STDERR):
    printColorCant((uint8_t *)message, length, ERRCOLORFONT, ERRCOLORBACK);
    ncNewline();
    break;
  }
}

int sys_read(uint64_t fd, uint64_t buffer, uint64_t length) {
  int retVal = 0;
  switch (fd) {
  case (STDIN):
    retVal = read_chars((char *)buffer, length);
    break;
  }
  return retVal;
}

int read_chars(char *buffer, int length) {
  int chars_read = 0;
  for (int i = 0; i < length; i++) {
    chars_read++;
    buffer[i] = read_key();
    if (buffer[i] == 0) {
      i = length; // si llego a un null dejo de leer
      chars_read--;
    }
  }
  return chars_read;
}

void sys_sleep(uint32_t  cant,uint32_t  unidad){
  print("hola");
  //sleep(cant);
  tickscounter(unidad); 
  print("adios");
}

/*
struct timeval {
               time_t      tv_sec;     // seconds 
               suseconds_t tv_usec;    // microseconds 
           };

struct timezone {
               int tz_minuteswest;     // minutes west of Greenwich 
               int tz_dsttime;         // type of DST correction 
           }

int sys_gettimeofday(timeval *hora, timezone *zona){

 }
 */