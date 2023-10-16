#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <ourlib.h>

void keyboard_handler(){
    ncPrintHex(_getKey());
}