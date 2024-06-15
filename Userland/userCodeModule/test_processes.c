#include <stdio.h>
#include "include/syscall.h"
#include "include/test_util.h"
#include <stddef.h>
#include "include/commands.h"
#include "include/userlib.h"
#include "include/userlibasm.h"

typedef enum TestState {
    TRUNNING,
    TBLOCKED,
    TKILLED 
}TestState;

typedef struct TestProcess {
  pid_t pid;
  TestState state;
} TestProcess;

void local_endless_loop() {
  while (1)
    ;
}

int64_t test_processes(uint64_t argc, char *argv[], int stdin, int stdout) {
    uint8_t proc_amount;
    uint8_t alive = 0;
    uint8_t action;
    uint64_t max_processes;
    char *argvAux[] = {0};
    boolean foreground = FALSE;   //los procesos no interactuan con stdout

    if (argc != 1)
        return -1;


    max_processes = satoi(argv[1]);             //argv[1] es el primer argumento del comando con nombre argv[0]
    if (max_processes <= 0 || max_processes>20)    
        return -1;

    TestProcess procs[max_processes];

    while (1) {

        // Create max_processes processes
        for (proc_amount = 0; proc_amount < max_processes; proc_amount++) {
        argvAux[0] = "endless_loop";
        procs[proc_amount].pid = create_shiny_process(&local_endless_loop, 1, argvAux, DEFAULT_PRIORITY, FALSE, foreground ,stdin, stdout);
        if (procs[proc_amount].pid == -1) {
            print("test_processes: ERROR creating process\n");
            return -1;
        } 
        else {
            procs[proc_amount].state = TRUNNING;
            alive++;
            print("New process created with pid: ");
            char aux[5] = {};

            print()
        }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0) {

        for (proc_amount = 0; proc_amount < max_processes; proc_amount++) {
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