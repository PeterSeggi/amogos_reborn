#include <stdint.h>
#include <stdio.h>
#include "include/commands.h"
#include "include/syscall.h"
#include "include/test_util.h"

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;


  sem_t * mySem;
  if (use_sem){
    mySem = sem_open(SEM_ID, 1);
    if (!mySem) {
      print("test_sync: ERROR opening semaphore\n");
      return -1;
    }
  }
  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sem_down(mySem);
    slowInc(&global, inc);
    if (use_sem)
      sem_up(mySem);
  }

  if (use_sem)
    sem_close(mySem);

  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[], int stdin, int stdout) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = create_shiny_process(&my_process_inc, 3, argvDec, DEFAULT_PRIORITY, FALSE, TRUE, stdin, stdout);
    pids[i + TOTAL_PAIR_PROCESSES] = create_shiny_process(&my_process_inc, 3, argvInc, DEFAULT_PRIORITY, FALSE, TRUE, stdin, stdout);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    waitpid(pids[i]);
    waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  char aux[BUFFER_SIZE] = {0};
  uintToBase(global, aux, 10);
  print("Final value: ");
  print(aux);
  print("\n");

  return 0;
}
