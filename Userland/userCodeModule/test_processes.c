#include <stdio.h>
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
    boolean foreground = FALSE;   //processes wont interact with stdout

    if (argc != 2){
        print("test_proc: Wrong argument count...\n");
        print("test_proc: Use: testproc <max_processes>\n");
        return -1;
    } 

    max_processes = satoi(argv[1]);
    if (max_processes <= 0 || max_processes>20){
        print("test_proc: Max processes must be a positive number, lesser than 20\n");
        return -1;
    }

    TestProcess procs[max_processes];

    while (1) {

        // Create max_processes processes
        for (proc_amount = 0; proc_amount < max_processes; proc_amount++) {
            argvAux[0] = "e_loop";
            procs[proc_amount].pid = create_shiny_process(&local_endless_loop, 1, argvAux, DEFAULT_PRIORITY, FALSE, foreground ,stdin, stdout);
            if (procs[proc_amount].pid == -1) {
                print("test_proc: ERROR creating process\n");
                return -1;
            } 
            else {
                procs[proc_amount].state = TRUNNING;
                alive++;
                print("New process created with pid: ");
                printDec(procs[proc_amount].pid);
                print("\n");
            }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0) {

            for (proc_amount = 0; proc_amount < max_processes; proc_amount++) {
                action = GetUniform(100) % 2;
                switch (action) {
                    case 0:
                        if (procs[proc_amount].state == TRUNNING || procs[proc_amount].state == TBLOCKED) {
                            kill(procs[proc_amount].pid);
                            procs[proc_amount].state = TKILLED;
                            alive--;
                            print("Killed: ");
                            printDec(procs[proc_amount].pid);
                            print("\n");
                        }
                        break;

                    case 1:
                        if (procs[proc_amount].state == TRUNNING) {
                            block_proc(procs[proc_amount].pid);
                            procs[proc_amount].state = TBLOCKED;
                        }
                        print("Blocked: ");
                        printDec(procs[proc_amount].pid);
                        print("\n");
                        break;
                    }
            }

            // Randomly unblocks processes
            for (proc_amount = 0; proc_amount < max_processes; proc_amount++){
                if (procs[proc_amount].state == TBLOCKED && GetUniform(100) % 2) {

                    block_proc(procs[proc_amount].pid);
                    procs[proc_amount].state = TRUNNING;
                    print("Unblocked: ");
                    printDec(procs[proc_amount].pid);
                    print("\n");
                }
            }
        }
    }
}