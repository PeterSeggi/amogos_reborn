#ifndef MMAN_H
#define MMAN_H

#include <stdint.h>
#include <stddef.h>

void mm_init();

void * my_malloc(uint64_t size);
void my_free(void * addr_to_free);

uint64_t get_mem_total();
uint64_t get_mem_vacant();
uint64_t get_mem_occupied();

#endif