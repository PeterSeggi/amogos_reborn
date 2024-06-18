// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <mman.h>

//assumes free mem from 0x600000
//qemu flag gives 512M -> 0x20000000
#define FREE_MEM_START 0x600000
#define FREE_MEM_END 0x20000000

#define MEM_CHUNK 512 //multiple of 8 for alignment

/*
    WORDMAP: 2Byte maps MEM_CHUNK byte
    
     ________________ ____
    |________________|____|
    ^                ^
    start free_mem   header
*/

uint32_t * header = NULL;
int header_unit_size = sizeof(uint32_t);

uint64_t total_mem = 0;
uint64_t vacant_mem = 0;
uint64_t occupied_mem = 0;


void * assign_mem(uint32_t size);

void mm_init(){
    total_mem = (FREE_MEM_END - FREE_MEM_START);
    total_mem -= total_mem%MEM_CHUNK;
    total_mem -= (total_mem/MEM_CHUNK)*header_unit_size;
    vacant_mem = total_mem;
    occupied_mem = 0;
    header = (uint32_t *)(FREE_MEM_START + total_mem);
    for(int i = 0; (header+i) < (uint32_t *)FREE_MEM_END; i++){//fill manager with 0
        *(header+i)=0;
    }
}

void * my_malloc(uint32_t size){
    void *to_ret = NULL;

    if(size%MEM_CHUNK) size += (MEM_CHUNK - size%MEM_CHUNK);

    if(size && (get_mem_vacant() >= size)){
        to_ret = (void *) assign_mem(size/MEM_CHUNK);
    }

    return to_ret;
}

void my_free(void * addr_to_free){
    if(!addr_to_free) return;
    uint64_t aux_idx = ((uint64_t) addr_to_free - FREE_MEM_START)/MEM_CHUNK;
    uint64_t aux_size = *(header + aux_idx);

    *(header + aux_idx) = 0;

    vacant_mem += aux_size*MEM_CHUNK;
    occupied_mem -= aux_size*MEM_CHUNK;

    return;
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

/*----------------------
  | Local functions
  -----------------------*/

/**
*@brief     Looks for the first available space in the management structure.
*@param[in] size Desired amount of "MEM_CHUNK"s to be allocated.
*@return    Address for the desired size.
*@note      If a suitable address isn't found, returns null.
*/
void * assign_mem(uint32_t size){
    void *to_ret = NULL;

    uint8_t set = 0;
    uint64_t idx = 0;

    for(idx = 0; (idx < total_mem/MEM_CHUNK) && (!set);){
        if(!(*(header+idx))){
            uint64_t j = 0;
            while(!(*(header+idx+j)) && j<size && (idx+j) < total_mem/MEM_CHUNK) j++;
            if(j == size) set = 1;
            else idx += j;
        }
        else idx += *(header+idx);
    }
    if(set){
        *(header + idx) = size;
        to_ret = (void *)(FREE_MEM_START + idx*MEM_CHUNK);

        vacant_mem-=size*MEM_CHUNK;
        occupied_mem+=size*MEM_CHUNK;
    }

    return to_ret;
}
