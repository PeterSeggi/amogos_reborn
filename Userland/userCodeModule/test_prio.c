#include <stddef.h>
#include "include/test_util.h"
#include "include/userlibasm.h"
#include "include/userlib.h"

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 5
#define LOWEST 0  
#define LOW_MED 1  
#define MEDIUM 2 
#define HIGH_MED 3  
#define HIGHEST 4 

int64_t prio[TOTAL_PROCESSES] = {LOWEST, LOW_MED, MEDIUM, HIGH_MED, HIGHEST};

void local_endless_loop_print(int argc, char **argv) {
  if(argc!=2) exit();

  int64_t pid = get_pid();

  while (1) {
    printDec(pid);
    print("\n");
    bussy_wait(satoi(argv[1]));
  }
}

void test_prio(uint64_t argc, char *argv[], int stdin, int stdout) {
  int64_t pids[TOTAL_PROCESSES];
  int new_argc=2;
  char aux[20]={0};
  uintToBase(WAIT,aux,10);
  char *new_argv[]= {"endless_loop_print", aux};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++){
    pids[i] = create_shiny_process(&local_endless_loop_print, new_argc, new_argv, HIGHEST, FALSE, TRUE, stdin, stdout);
  }

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block_proc(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], MEDIUM);

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block_proc(pids[i]);

  bussy_wait(WAIT);
  print("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    kill(pids[i]);

  print("\nTEST FINSIHED\n");
}
