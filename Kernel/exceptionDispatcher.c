#include <exception_handler.h>
#include <stdint.h>

void exc_00();
void exc_06();

void exceptionDispatcher(uint64_t exc){
    switch(exc){
        case(0x00):
            exc_00();
            break;

        case(0x06):
            exc_06();
            break;
    }
    return;//???
}

void exc_00(){
    ZeroDivisionHandler();
}

void exc_06(){
    InvalidOpcodeHandler();
}