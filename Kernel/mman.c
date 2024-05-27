#include <mman.h>

//asumimos mem libre a partir de 0x600000
//como es de 512M el qemu, inyectamos el valor 0x20000000
#define FREE_MEM_START 0x600000
#define FREE_MEM_END 0x20000000

#define MEM_CHUNK 8 //queremos que de direcciones alineadas

/*
    WORDMAP: 2Byte mapean 8 byte(mem_chunk)
    
     ________________ ____
    |________________|____|
    ^                ^
    start free_mem   header
*/

uint16_t * header = NULL;

uint64_t total_mem = 0;
uint64_t vacant_mem = 0;
uint64_t occupied_mem = 0;

void * assign_mem(uint16_t size);

void mm_init(){
    total_mem = (FREE_MEM_END - FREE_MEM_START);//queremos multiplos de mem_chunk
    total_mem -= total_mem%MEM_CHUNK;
    total_mem -= (total_mem/MEM_CHUNK)*2;
    vacant_mem = total_mem;
    occupied_mem = 0;
    header = FREE_MEM_START + total_mem;
    for(int i = 0; (header+i) < FREE_MEM_END; i++){//set 0 de manager mem
        *(header+i)=0;
    }
}

void * my_malloc(uint16_t size){
    void *to_ret = NULL;

    size += (MEM_CHUNK - size%MEM_CHUNK);

    if(size && (get_mem_vacant() >= size)){//TODO: race condition (thread safe? in case multiple assign mem?)
        to_ret = (void *) assign_mem(size/MEM_CHUNK);
    }

    return to_ret;
}

void * assign_mem(uint16_t size){
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
        to_ret = FREE_MEM_START + idx*MEM_CHUNK;

        vacant_mem-=size*MEM_CHUNK;
        occupied_mem+=size*MEM_CHUNK;
    }

    return to_ret;
}

void my_free(void * addr_to_free){
    if(!addr_to_free) return;
    uint64_t aux_idx = ((uint64_t) addr_to_free - FREE_MEM_START);
    uint64_t aux_size = *(header + aux_idx);

    *(header + aux_idx) = 0;

    vacant_mem += aux_size*MEM_CHUNK;
    occupied_mem -= aux_size*MEM_CHUNK;

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

//TESTING
//TODO: esto no va en la entrega final!!
/*
int main(){
	mm_init();
	int *ptr = (int *) my_malloc(sizeof(int));

	if(!ptr) exit(1);

	*ptr = 5;


	int *ptr2 = (int *) my_malloc(sizeof(int));

	if(!ptr2) exit(1);

	*ptr2 = 7;

	my_free(ptr2);

    my_free(ptr);
}
*/