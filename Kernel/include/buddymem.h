#include <stdint.h>

#define EXIT_SUCCESS 0
#define MAP_LENGTH 15

#define EMPTY 0
#define FRAGMENTED 1 
#define FULL 2
#define ALLFULL 3

#define LEFT 0
#define RIGHT 1

#define NODE_AMOUNT 2097151


typedef struct memNode {
    char state;
} memNode;


void initializeMap();

void showMap();

uint64_t buddy_alloc(uint64_t size);
int buddy_free(uint64_t address);

void showMapDepth();

int findAndAlloc(uint64_t size, uint64_t start, uint64_t current_size, int index, uint64_t* found_address);

int findAndFree(uint64_t adddress, uint64_t start, uint64_t current_size, int index, int from);
