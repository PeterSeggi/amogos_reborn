#include <stdint.h>
#include <lib.h>
#include <naiveConsole.h>

int key_buf;

void key_handler(){
    key_buf = readKey();    
    ncPrint("Key");
}

