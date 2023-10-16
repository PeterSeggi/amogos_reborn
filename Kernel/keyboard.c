#include <stdint.h>
#include <lib.h>
#include <naiveConsole.h>

typedef void (*FuncPtr)();

char scan_chars[] = {
    '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\n',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'
};

int key_buf;
int write = 0;
int awaiting = 0;
int release;
FuncPtr release_func;

void key_handler(){
    key_buf = readKey();    
    if (write){
        ncPrintHex(key_buf);
    }

    if (awaiting){
        if (key_buf == release){
            release_func();
            awaiting = 0;
        }
    }
}

void set_await(int key, FuncPtr func){
    awaiting = 1;
    release = key;
    release_func = func;
}

void write_out(){
    write = 1;
}

void silent(){
    write = 0;
}
