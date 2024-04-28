#include <mman.h>

//asumimos mem libre a partir de 0x600000
#define FREE_MEM_START 0x600000
#define FREE_MEM_END 0x20000000

uint64_t current = 0;

uint64_t total_mem = 0;
uint64_t vacant_mem = 0;
uint64_t occupied_mem = 0;

void mm_init(){
    total_mem = FREE_MEM_END - FREE_MEM_START;
    vacant_mem = total_mem;
    occupied_mem = 0;
    current = FREE_MEM_START;
}

uint64_t get_mem_total(){
	return total_mem;
}

uint64_t get_mem_vacant(){
	return vacant_mem;
}

uint64_t get_mem_occupied(){
	return occupied_mem;
}