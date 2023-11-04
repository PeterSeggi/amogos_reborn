#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

static void zero_division();
static void invalid_opcode();

void exceptionDispatcher(int exception) {
	switch(exception){
		case(ZERO_EXCEPTION_ID):
			zero_division();
			break;

		case(INVALID_OPCODE_ID):
			invalid_opcode();
			break;
	}
}

static void zero_division() {
	printColor("zero??",ERRCOLORFONT,ERRCOLORBACK);
}

static void invalid_opcode() {
	printColor("invalidOpcode!",ERRCOLORFONT,ERRCOLORBACK);
}