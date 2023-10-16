#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <ourlib.h>

void keyboard_handler(){
    //ncPrint(scancodeToString(_getKey()));
    ncPrintHex(_getKey());
}