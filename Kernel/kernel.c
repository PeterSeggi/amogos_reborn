// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include <pipe.h>
#include <stdint.h>
#include <sound.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <videoDriver.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <keyboard.h>
#include <interrupts.h>
#include <mman.h>
#include <processManager.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const userspaceAddress = (void*)0x400000;
static void * const dataspaceAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		userspaceAddress,
		dataspaceAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

    ncClear();
	return getStackBase();
}

int main()
{	
	_cli();
    load_idt();
    flush_buffer();
	mm_init();
	initializeSleepingTable();
	initialize_pcb();
    init_pipe();
	initializeScheduler();
    key_init();

	//_sti();
	//while(1){
	//	_hlt();
	//}
    
	

	//fin pruebas
	//_setUser();

//    Esto no hace falta porque el salto se hace en set user
//    ((EntryPoint) userspaceAddress)();
    
	return 0;

}
