#include <videoDriver.h>

#define ZERO_EXCEPTION_ID  0
#define INVALID_OP_CODE_ID 6

static void zero_division();
static void invalid_op_code();


void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();

    else if (exception == INVALID_OP_CODE_ID)
		invalid_op_code();
}

static void zero_division() {
    printError("\nOh no! Al parecer trataste de desafiar la matematica y saliste perdiendo.\nTe dejo el RIP aca de cuando el programa hizo rip: ");
    printDecError(_getRip());
    printError("\nRedireccionandote a una shell en:\n");
    sleep(1, 0);
    printError("5...\n");
    sleep(1, 0);
    printError("4...\n");
    sleep(1, 0);
    printError("3...\n");
    sleep(1, 0);
    printError("2...\n");
    sleep(1, 0);
    printError("1...\n");
    sleep(1, 0);
}

static void invalid_op_code(){
    printError("\nOh no! Al parecer trataste de reescribir asm y te diste cuenta que no le pinto nada a la compu.\nTe dejo el RIP de cuando el programa hizo rip: ");
    printDecError(_getRip());
    printError("\nRedireccionandote a una shell para que sigas mandandote macanas en:\n");
    sleep(1, 0);
    printError("5...\n");
    sleep(1, 0);
    printError("4...\n");
    sleep(1, 0);
    printError("3...\n");
    sleep(1, 0);
    printError("2...\n");
    sleep(1, 0);
    printError("1...\n");
    sleep(1, 0);
}
