#include <teclado.h>
#include <naiveConsole.h>

void key_handler(){
    ncPrintHex(getKey());
}