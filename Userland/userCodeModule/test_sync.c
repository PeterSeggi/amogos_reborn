
#include <stddef.h>
#include "include/test_util.h"
#include "include/userlibasm.h"
#include "include/userlib.h"
#include "include/commands.h"

#define SEM_ID "test_sync_sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 4){
    print("test_sync: Wrong amount of parameters\n");
    exit();
  }

  if ((n = satoi(argv[1])) <= 0){
    print("test_sync: Wrong amount of inc\n");
    exit();
  }
  if ((inc = satoi(argv[2])) == 0){
    print("test_sync: Invalid kind of inc\n");
    exit();
  }
  if ((use_sem = satoi(argv[3])) < 0){
    print("test_sync: Invalid use_sem\n");
    exit();
  }

  sem_t *aux_sem ;

  if (use_sem){
    aux_sem = sem_open(SEM_ID, 1);
    if (!aux_sem) {
      print("test_sync: ERROR opening semaphore\n");
      exit();
    }
  }
    

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem) sem_down(aux_sem);
    slowInc(&global, inc);
    if (use_sem) sem_up(aux_sem);
  }

  if (use_sem) sem_close(aux_sem);

  exit();
}

void test_sync(uint64_t argc, char *argv[], int stdin, int stdout){ //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3){
    print("test_sync: Wrong amount of parameters\n");
    return;
  }

  char *argvDec[] = {"test_sync_dec", argv[1], "-1", argv[2]};
  char *argvInc[] = {"test_sync_inc", argv[1], "1", argv[2]};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = create_shiny_process(&my_process_inc, 4, argvDec, 4, FALSE, TRUE, stdin, stdout);
    pids[i + TOTAL_PAIR_PROCESSES] = create_shiny_process(&my_process_inc, 4, argvInc, 4, FALSE, TRUE, stdin, stdout);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    waitpid(pids[i]);
    waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  print("Final value: ");
  if(global<0){
    print("-");
    printDec((uint64_t)(global*(-1)));
  }
  else  printDec(global);
  print("\n");

  return;
}
