#include <stdio.h>
#include "include/syscall.h"
#include "include/test_util.h"
#include <stddef.h>
#include "include/commands.h"
#include "include/userlib.h"
#include "include/userlibasm.h"

enum ThisState { THISRUNNING,
             THISBLOCKED,
             THISKILLED };

typedef struct P_rq {
  int32_t pid;
  enum ThisState state;
} p_rq;

int64_t test_processes(uint64_t argc, char *argv[], int stdin, int stdout) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};

  if (argc != 1)
    return -1;

  if ((max_processes = satoi(argv[0])) <= 0)
    return -1;

  p_rq p_rqs[max_processes];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      argvAux[0] = "endless_loop";
      p_rqs[rq].pid = create_shiny_process(&endless_loop, 1, argvAux, DEFAULT_PRIORITY, FALSE, TRUE,stdin, stdout);

      if (p_rqs[rq].pid == -1) {
        print("test_processes: ERROR creating process\n");
        return -1;
      } else {
        p_rqs[rq].state = THISRUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == THISRUNNING || p_rqs[rq].state == THISBLOCKED) {
              kill(p_rqs[rq].pid);
              p_rqs[rq].state = THISKILLED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == THISRUNNING) {
              block_proc(p_rqs[rq].pid);
              p_rqs[rq].state = THISBLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == THISBLOCKED && GetUniform(100) % 2) {
          block_proc(p_rqs[rq].pid);
          p_rqs[rq].state = THISRUNNING;
        }
    }
  }
}
