#include <keyboard.h>
#include <lib.h>
#include <stdint.h>
#include <syscall_handler.h>
#include <videoDriver.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define ERRCOLORFONT 0xFF0000 // texto rojo, bg gris;
#define ERRCOLORBACK 0xDADADA // texto rojo, bg gris;

void syscall_handler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax) {
  switch (rax) {

  case (0x00):
    sys_read(rdi, rsi, rdx);
    break;

  case (0x01):
    sys_write(rdi, rsi, rdx);
    break;

  case (0x23):
    sys_nanosleep(rdi);
    break;

  case (0x77):
    sys_draw(rdi, rsi, rdx, rcx, r8);
    break;

  case (0x83):
    sys_screenData(rdi, rsi, rdx, rcx);
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

void sys_nanosleep(uint64_t nanos){
  milisleep(nanos);
}

void sys_draw(uint64_t bitmap, uint64_t hexColor, uint64_t height, uint64_t init_x, uint64_t init_y){
  changeDrawSize(3);
  printBitmap(bitmap, hexColor, height, init_x, init_y);
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
