#include "include/test_util.h"
#include "include/userlib.h"
#include "include/userlibasm.h"
//#include <stddef.h>

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint16_t size;
} mm_rq;

void * u_memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

uint64_t test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint64_t rq;
  uint64_t total;
  uint64_t max_memory;

  if (argc != 2)
    return -1;

  if ((max_memory = satoi(argv[1])) <= 0)
    return -1;

  // Conversion de megas a bytes
  max_memory = max_memory * 1024 * 1024;

  int test_loop = 0;

  while (1) {
    rq = 0;
    total = 0;

    print("-----------------------------------------------\n");
    print("Test n:");
    printDec(test_loop++);
    print("\n");

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = my_malloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    printDec(rq);
    print(" blocks allocated. Memory ");
    printDec(total);
    print("B/");
    printDec(max_memory);
    print("B\n");
    print("Setting... ");

    // Set
    uint64_t i;
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address)
        u_memset(mm_rqs[i].address, i, mm_rqs[i].size);
       
    }
    print("done!\n");

    print("Testing... ");

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          print("test_mm ERROR\n");
          return -1;
        }

    print("done!\n");
    print("Freeing.. ");

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        my_free(mm_rqs[i].address);

    print("done!\n");
    print("Test ");
    printDec(test_loop - 1);
    print(" succesful!\n\n\n");
  }
}
