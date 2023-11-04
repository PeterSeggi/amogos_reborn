#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0

static void zero_division();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
}

static void zero_division() {
    printError("\nOh no! Al parecer trataste de desafiar la matematica y saliste perdiendo.\nRedireccionandote a una shell en:\n");
    sleep(2, 0);
    printError("3...\n");
    sleep(2, 0);
    printError("2...\n");
    sleep(2, 0);
    printError("1...\n");
    sleep(2, 0);
}
