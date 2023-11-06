#include <stdint.h>
#include <videoDriver.h>
#include <keyboard.h>

#define ZERO_EXCEPTION_ID  0
#define INVALID_OP_CODE_ID 6

static void zero_division();
static void invalid_op_code();

extern uint64_t regsBuf[];
static void showRegs();
char* regsNames[18] = {"rax:", "rbx:", "rcx:", "rdx:", "rsi:", "rdi:", "rbp:", "rsp:", "r8:", "r9:",
                       "r10:", "r11:", "r12:", "r13:", "r14:", "r15:", "rip:", "rflags:"};

char escape[1];


void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID){
        clear();
		zero_division();
    }

    else if (exception == INVALID_OP_CODE_ID){
        clear();
		invalid_op_code();
    }
}

static void zero_division() {
    printError("Oh no! Al parecer trataste de desafiar la matematica y saliste perdiendo.Te dejo los registros de cuando el programa exploto:\n");
    showRegs();
    printError("Cuando estes listo y preparado para volver al main (altamente recomendable) presiona cualquier tecla.");
    while(read_key(0) == 0){
        sleep(0,0);
    }
}

static void invalid_op_code(){
    printError("Oh no! Al parecer trataste de reescribir asm y te diste cuenta que no le pinto nada a la compu. Te dejo los regs de cuando el programa hizo rip:\n");
    showRegs();
    printError("Cuando estes listo y preparado para volver al main (altamente recomendable) presiona cualquier tecla.");
    while(read_key(0) == 0){
        sleep(0,0);
    }
}


static void showRegs(){
    for (int i = 0; i < 18; i++){
        printError(regsNames[i]);
        printDecError(regsBuf[i]);
        print("\n");
    }

}
