#include <lib.h>
#include <naiveConsole.h>
#include <syscall_handler.h>
#include <stdint.h>

#define STDOUT 1
#define STDERR 2
#define ERRCOLOR 0x74 // texto rojo, bg gris;

void syscall_handler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax) {
  switch (rax) {
  case (0x01):
    sys_write(rdi, rsi, rdx);
    break;
  }
}

void sys_write(uint64_t fd, uint64_t message, uint64_t length){
    switch (fd) {
        case (STDOUT):
            ncPrintCant((char*) message, length);
            ncNewline();
            break;

        case (STDERR):
            ncPrintColorCant(message, length, ERRCOLOR);
            ncNewline();
            break;
    }
}
