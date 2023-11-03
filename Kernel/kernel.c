#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <ourlib.h>
#include <videoDriver.h>
#include <time.h>
#include <idtLoader.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

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
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

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
	ncNewline();

	ncPrint("[Initializing IDT]");
	ncNewline();
	load_idt();
	ncPrint("[Done]");
	ncNewline();

	ncNewline();
	return getStackBase();
}

int main()
{	
	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	ncPrint("[Finished]");
	ncNewline();

	//bokita code (funca en video mode, no textmode)

	//draw_rectangle(300,35,0x000000FF,0,0);
	//draw_rectangle(300,30,0x00FFFF00,0,35);
	//draw_rectangle(300,35,0x000000FF,0,65);

	//writeSky();
	//writeStar();

	//sleep(5);
	
	ncClear();

	ncPrint("Welcome Back!");
	ncNewline();
	ncPrint("1 - 1");
	ncNewline();
	miniCalcu("1 - 1");
	ncNewline();
	ncPrint("2 + 2");
	ncNewline();
	miniCalcu("2 + 2");
	ncNewline();
	ncPrint("5 * 3");
	ncNewline();
	miniCalcu("5 * 3");
	ncNewline();
	ncPrint("4 / 2");
	ncNewline();
	miniCalcu("4 / 2");
	ncNewline();
	ncPrint("1 / 0");
	ncNewline();
	miniCalcu("1 / 0");
	ncNewline();


	//ncPrint("Pooling: Presione una tecla para ver su scancode");
	//ncNewline();
	//ncPrintHex(_getKey());

	while(1);

	return 0;
}
