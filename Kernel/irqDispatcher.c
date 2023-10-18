#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <syscall_handler.h>
#include <naiveConsole.h>

static void int_20();
static void int_21();
static void int_80();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
        case 1:
            int_21();
            break;
        case 128:
            int_80();
            break;
        ncPrintDec(irq);
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21() {
    key_handler();
}

void int_80() {
    syscall_handler();
}
