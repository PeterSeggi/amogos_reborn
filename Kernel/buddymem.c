#include "mman.h"
#include <stdint.h>
#include <stdio.h>


#define EXIT_SUCCESS 0
#define MAP_LENGTH 15
#define EMPTY '0'
#define FRAGMENTED '1'
#define FULL '2'
#define ALLFULL '3'
#define LEFT 0
#define RIGHT 1


#define FREE_MEM_START 0x600000
#define FREE_MEM_END 0x20000000

typedef struct memNode {
    char state;
} memNode;

int findAndAlloc(uint64_t size, uint64_t start, uint64_t current_size, int index, uint64_t* found_address);
int findAndFree(uint64_t adddress, uint64_t start, uint64_t current_size, int index, int from);

uint64_t start_location = 0;
uint64_t memSize = 64;
uint64_t minSize = 8;

memNode memMap[MAP_LENGTH];

int main(int argc, char *argv[])
{
    initializeMap();

    uint64_t newLocation;

    newLocation = buddy_alloc(8);
    printf("%lu\n", newLocation);


    uint64_t secondLocation = buddy_alloc(8);
    printf("%lu\n", secondLocation);
    showMap();

    uint64_t thirdLocation = buddy_alloc(16);
    printf("%lu\n", thirdLocation);
    showMap();

    // newLocation = buddy_alloc(16);
    // printf("%lu\n", newLocation);

    // newLocation = buddy_alloc(16);
    // printf("%lu\n", newLocation);

    // newLocation = buddy_alloc(16);
    printf("liberamos!\n");


    buddy_free(newLocation);
    showMap();
    buddy_free(secondLocation);
    showMap();
    buddy_free(thirdLocation);
    showMap();

    return EXIT_SUCCESS;
}

memNode newNode()
{
    memNode toRet;
    toRet.state = EMPTY;
    return toRet;
}

void initializeMap()
{
    for (int i = 0; i < MAP_LENGTH; i++)
    {
        memMap[i] = newNode();
    }
}

void showMap()
{
    printf("[");
    for (int i = 0; i < MAP_LENGTH; i++)
    {
        printf("%d: %c", i, memMap[i].state);
        if (i != MAP_LENGTH - 1)
        {
            printf(", ");
        }
    }
    printf("]\n\n");
}

void showMapDepth()
{
}

uint64_t buddy_alloc(uint64_t size)
{
    if (size < minSize)
        size = minSize;

    uint64_t toRet = 0;
    if (findAndAlloc(size, start_location, memSize, 0, &toRet))
    {
        // printf("%lu\n", toRet);
        return toRet;
    };
    return 0;
}

int buddy_free(uint64_t address)
{
    findAndFree(address, start_location, memSize, 0, LEFT);
    return 0;
}

int findAndAlloc(uint64_t size, uint64_t start, uint64_t current_size, int index, uint64_t *found_address)
{
    //        printf("Buscamos %lu en %d con current_size %lu y direccion inicial %lu\n", size, index, current_size, start);

    if (size > current_size / 2)
    {
        // printf("Encontramos en %d! y estaba %c\n", index, memMap[index].state);
        if (memMap[index].state == EMPTY)
        {
            //            printf("Insertamos en %d! que va desde %lu hasta %lu\n", index, start, start + current_size - 1);
            memMap[index].state = FULL;
            *found_address = start;
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

int findAndFree(uint64_t address, uint64_t start, uint64_t current_size, int index, int from)
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
            // printf("Liberamos!\n");
            memMap[index].state = EMPTY;
            return 1;
        }

        // go left
        // printf("Vamos left!\n");

        findAndFree(address, start, current_size / 2, 2 * index + 1, LEFT);
    }

    else
    {
        // go right
        // printf("Vamos right!\n");

        findAndFree(address, start + current_size / 2, current_size / 2, 2 * index + 2, LEFT);
    }

    // cascade upwards checking buddy status
    if (memMap[2 * index + 1].state == EMPTY && memMap[2 * index + 2].state == EMPTY)
    {
        printf("Cambiamos %d a empty!\n", index);

        memMap[index].state = EMPTY;
    }

    // else{
    //     printf("En %d estan l: %d y r: %d", index, memMap[2 * index + 1].state, memMap[2 * index + 2].state);
    // }

    return 1;
}
