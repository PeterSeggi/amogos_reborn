#include <mman.h>

//asumimos mem libre a partir de 0x600000
//como es de 512M el qemu, inyectamos el valor 0x20000000
#define FREE_MEM_START 0x600000
#define FREE_MEM_END 0x20000000

uint64_t * header = NULL;

uint64_t total_mem = 0;
uint64_t vacant_mem = 0;
uint64_t occupied_mem = 0;

void * assign_mem(uint64_t size);

void mm_init(){
    total_mem = (FREE_MEM_END - FREE_MEM_START)/2;
    vacant_mem = total_mem;
    occupied_mem = 0;
    header = FREE_MEM_START + total_mem;
}

void * my_malloc(uint64_t size){
    void *to_ret = NULL;

    if(get_mem_vacant() >= size){//TODO: race condition (thread safe? in case multiple assign mem?)
        to_ret = (void *) assign_mem(size);
    }

    return to_ret;
}

void * assign_mem(uint64_t size){
    void *to_ret = NULL;

    uint8_t set = 0;
    uint64_t idx = 0;

    for(idx = 0; (idx < total_mem) && (!set);){//TODO: no i++, sino i += *(header+i) es mas eficiente
        if(!(*(header+idx))){
            uint64_t j = 0;
            while(!(*(header+idx+j))) j++;
            if(j == size) set = 1;
            else idx += j;
        }
        else idx++;
    }
    if(set){
        uint64_t j = 0;
        while(j<size){
            *(header+idx+j) = 1;
            j++;
        }
        *(header+idx) = size;
        to_ret = FREE_MEM_START + idx;

        vacant_mem-=size;
        occupied_mem+=size;
    }

    return to_ret;
}

void my_free(void * addr_to_free){
    uint64_t aux_idx = (addr_to_free-FREE_MEM_START);
    uint64_t aux_size = *(header+aux_idx);
    //if(aux_size) TODO:error check
    while(aux_size){
        *(header+aux_idx+aux_size-1) = 0;
        aux_size--;
    }

    vacant_mem+=aux_size;
    occupied_mem-=aux_size;

    addr_to_free = NULL;

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