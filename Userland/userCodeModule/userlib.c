#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/file_descriptors.h"

static char buffer[64] = {'0'};

//================================================================================================================================
// Writting
//================================================================================================================================
//================================================================================================================================

void print(char * string){
    _print(STDOUT, string, strlen(string));
}

void printError(char * string){
    _print(STDERROR, string, strlen(string));
}

int read(char* buffer, int length){
    return _read(STDIN, buffer, length);
}

int strlen(char * string){
    int i=0;
    while(string[i++]!=0);
    return i;
}

void printBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    print(buffer);
}

void printDec(uint64_t value){
    printBase(value, 10);
}


//================================================================================================================================
// Number handling
//================================================================================================================================
//================================================================================================================================
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}


//================================================================================================================================
// Sleep
//================================================================================================================================

void sleep(uint32_t cant, uint32_t unidad){
	_sleep(cant, unidad);
}

//================================================================================================================================
// Clock
//================================================================================================================================
void getClock(int *hrs, int *min, int *seg){
	_getClock(hrs, min, seg);
}

void printClock(){
	int hrs;
    int min;
    int seg;
    print("\n");
    getClock(&hrs, &min, &seg);
    print("La hora es...");
    if(hrs<10) print("0");
    printDec(hrs);
    print(":");
    if(min<10) print("0");
    printDec(min);
    print(":");
    if(seg<10) print("0");
    printDec(seg);
    print("\n");
}

//================================================================================================================================
// Registers
//================================================================================================================================

void getRax(int *rax){
	_getRax(rax);
}

void printRegisters(){
	//no estoy incluyendo los de 512/256/128/32/16-bit ni RFLAGS 
	struct registers regsStruct;
	_getRegisters(&regsStruct);
	int values[]={regsStruct.rax, regsStruct.rbx, regsStruct.rcx, regsStruct.rdx, regsStruct.rbp, regsStruct.rsp,
					regsStruct.rsi, regsStruct.rdi, regsStruct.rip, regsStruct.r8, regsStruct.r9, regsStruct.r10, regsStruct.r11, 
					regsStruct.r12, regsStruct.r13, regsStruct.r14, regsStruct.r15};
	char* names[]={"rax:", "rbx:", "rcx:", "rdx:", "rbp:", "rsp:", "rsi:", "rdi:", "rip:",
				 	"r8:", "r9:", "r10:", "r11:", "r12:", "r13:", "r14:", "r15:"};
	int cantRegisters=17;
	for(int i=0; i<cantRegisters; i++){
		print(names[i]);
		printDec(values[i]);
		if(i%6==0 && i!=0){
			print("\n");
		}
		print("  ");
	}	
}