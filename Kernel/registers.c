#include <videoDriver.h>

long registros[18];
int saved=0;

int getRegs(long regs[]){
    if(!saved){
        return 0;
    }

    for (int i=0; i<18; i++) {
        regs[i]=registros[i];
    }
    return 1;
}

void regs_handler(){
    long* ptr = _regsInterrupt();
    for (int i=0; i<18; i++) {
        registros[i]=ptr[i];
    }
    saved=1;
}
