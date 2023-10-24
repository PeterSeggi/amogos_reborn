#include "include/lib.h"
#include "include/videoDriver.h"
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <videoDriver.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <keyboard.h>

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
    
    /* 
    // no borro esto porlas por si necesitamos para debugging later
	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
    */

    ncClear();
	return getStackBase();
}

int main()
{	
    load_idt();

    //printColorCant("bokesooooooo",5,0x07B0CA,0xB0CA07);
	//printColor("holiwiws",0x07B0CA,0xB0CA07);
    clear();
    ((EntryPoint) userspaceAddress)();
    
    /*
	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)dataspaceAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)dataspaceAddress);
	ncNewline();

    */
    
	return 0;

}
