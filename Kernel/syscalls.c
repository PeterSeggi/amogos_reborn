#include <syscalls.h>
#include <naiveConsole.h>
#include <ourlib.h>

#define STDOUT 1
#define STDERR 2
#define ERRCOLOR 0x74 //texto rojo, bg gris;

void syscall_handler(uint64_t id){
    switch(id){
        case(0x04):
            sys_write();
            break;
    }
}

void sys_write(){
    char * toWrite = _getCReg();
    uint64_t fd = _getBReg();
    uint64_t length = _getDReg();
    switch(fd){
        case(STDOUT):
            ourPrintCant(toWrite, length);
            ncNewline();
            break;
        case(STDERR):
            ourPrintCantColor(toWrite, length, ERRCOLOR);
            ncNewline();
            break;
    }
}