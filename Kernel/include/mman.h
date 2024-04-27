#ifndef MMAN_H
#define MMAN_H

#include <stdint.h>

uint64_t get_mem_total();
uint64_t get_mem_vacant();
uint64_t get_mem_occupied();

#endif