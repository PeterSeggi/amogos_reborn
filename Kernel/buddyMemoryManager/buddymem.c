#include "mman.h"
#include <stdint.h>
#include <stdio.h>


#define EXIT_SUCCESS 0

#define EMPTY 0
#define FRAGMENTED 1
#define FULL 2
#define ALLFULL 3

#define NODE_AMOUNT 2097151

#define FREE_MEM_START 0x10000000
#define FREE_MEM_END 0x20000000

#define MEM_SIZE 0x10000000 

#define MIN_SIZE 256



typedef struct memNode {
    char state;
} memNode;


int findAndAlloc(uint32_t size, uint64_t start, uint64_t current_size, int index, uint64_t* found_address);
int findAndFree(uint64_t adddress, uint64_t start, uint64_t current_size, int index);

memNode memMap[NODE_AMOUNT];
uint64_t free_mem = MEM_SIZE;
uint64_t occupied_mem = 0;

memNode newNode()
{
    memNode toRet;
    toRet.state = EMPTY;
    return toRet;
}

void mm_init()
{
    for (int i = 0; i < NODE_AMOUNT; i++)
    {
        memMap[i] = newNode();
    }
}

void * my_malloc(uint32_t size)
{
    if (size < MIN_SIZE)
        size = MIN_SIZE;

    void * toRet = NULL;
    uint64_t found_address = 0;
    if (findAndAlloc(size, FREE_MEM_START, MEM_SIZE, 0, &found_address))
    {
        toRet = (void *) found_address;
        return toRet;
    };
    return toRet;
}

void my_free(void * address)
{
    if (address == NULL){
        return;
    }
    findAndFree((uint64_t) address, FREE_MEM_START, MEM_SIZE, 0);
}


uint64_t get_mem_total(){
    return MEM_SIZE;
}

uint64_t get_mem_vacant(){
    return free_mem;
}


uint64_t get_mem_occupied(){
    return occupied_mem;
}


/*----------------------
  | Local functions
  -----------------------*/

int findAndAlloc(uint32_t size, uint64_t start, uint64_t current_size, int index, uint64_t* found_address)
{
    if (size > current_size / 2)
    {
        if (memMap[index].state == EMPTY)
        {
            memMap[index].state = FULL;
            *found_address = start;
            occupied_mem += current_size;
            free_mem -= current_size;
            return 1;
        }
        else
        {
            return 0;
        }
    }

    char currentState = memMap[index].state;
    int inserted = 0;

    if (currentState == EMPTY)
    {
        memMap[index].state = FRAGMENTED;
    }

    else if (currentState == FULL || currentState == ALLFULL)
    {
        return 0;
    }

    if (findAndAlloc(size, start, current_size / 2, index * 2 + 1, found_address))
    {
        inserted = 1;
        return 1;
    }

    else if (findAndAlloc(size, start + current_size / 2, current_size / 2, index * 2 + 2, found_address))
    {
        inserted = 1;
        return 1;
    }

    if (!inserted)
        memMap[index].state = ALLFULL;

    return 0;
}

int findAndFree(uint64_t address, uint64_t start, uint64_t current_size, int index)
{
    char current_state = memMap[index].state;

    // if allfull -> fragmented
    if (current_state == ALLFULL)
        memMap[index].state = FRAGMENTED;

    if (address < start + current_size / 2)
    {

        // if full we free and return
        if (current_state == FULL)
        {
            memMap[index].state = EMPTY;
            occupied_mem -= current_size;
            free_mem += current_size;
            return 1;
        }

        // go left
        findAndFree(address, start, current_size / 2, 2 * index + 1);
    }

    else
    {
        // go right
        findAndFree(address, start + current_size / 2, current_size / 2, 2 * index + 2);
    }

    // cascade upwards checking buddy status
    if (memMap[2 * index + 1].state == EMPTY && memMap[2 * index + 2].state == EMPTY)
    {
        memMap[index].state = EMPTY;
    }

    return 1;
}


