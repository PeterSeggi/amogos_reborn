#include "syscall.h"
#include "include/test_util.h"
#include "include/userlib.h"

#define MAX_BLOCKS 24772608 //for half the memory (aprox 189MB)

typedef struct MM_rq {
  void *address;
  uint16_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint64_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 2){
    print("Please correct your input\n");
    exit();
  }
    

  if ((max_memory = satoi(argv[1])) <= 0){
    print("Max memory must be positive");
    exit();
  }
    

  //MB to Bytes
  max_memory = max_memory*1024*1024;

  print("\n----------TEST_MM START----------\n");

  int test_loop = 0;
  while (1) {
    rq = 0;
    total = 0;

    print("Test n:");
    printDec(test_loop);
    print("\n");

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = my_malloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        print("malloc size:");
        printDec(mm_rqs[rq].size);
        print("total size:");
        printDec(total);
        print("/");
        printDec(max_memory);
        print("\n");
        rq++;
      }
    }

    print("bloques creados:");
    printDec(rq);
    print("\n");

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          print("test_mm ERROR\n");
          exit();
        }

    print("Ahora hacemos free\n");

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        my_free(mm_rqs[i].address);
    
    print("\n----------TEST_MM N: ");
    printDec(test_loop++);
    print("END----------\n");
  }
}