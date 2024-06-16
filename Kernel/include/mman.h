#ifndef MMAN_H
#define MMAN_H

#include <stdint.h>
#include <stddef.h>

/**
*@brief     Initializes the management structure and other variables.
*/
void mm_init();

/**
*@brief     Allocates a desired amount of bytes in memory.
*@param[in] size Size, in bytes, of the memory to be allocated.
*@return    Address of the allocated memory.
*@note      If a suitable address isn't found, returns null.
*/
void * my_malloc(uint32_t size);

/**
*@brief     Frees previously allocated memory.
*@param[in] addr_to_free Address to be freed.
*@note      If addr_to_free is null, simply returns.
*/
void my_free(void * addr_to_free);


/**
*@brief     Getter for the total amount of memory.
*@return    Amount of memory in total.
*/
uint64_t get_mem_total();

/**
*@brief     Getter for the total amount of memory vacant for allocations.
*@return    Current amount of memory vacant for allocations.
*/
uint64_t get_mem_vacant();

/**
*@brief     Getter for the total amount of memory occupied.
*@return    Current amount of memory occupied.
*/
uint64_t get_mem_occupied();

#endif
