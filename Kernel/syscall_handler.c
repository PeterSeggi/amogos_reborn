#include <naiveConsole.h>
#include <lib.h>

void syscall_handler(){
    ncPrint("int 0x80!");
}
