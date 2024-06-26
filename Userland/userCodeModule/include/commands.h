#ifndef COMMANDS_H
#define COMMANDS_H

#include "sh.h"
#include "userlibasm.h"
#include "userlib.h"
#include <stdint.h>

#define BUFFER_SIZE 128
#define DEFAULT_PRIORITY 4

// Utilidad general
pid_t init_ps(int read_fd, int write_fd, boolean foreground);
pid_t init_loop(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_mem(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);

// Procesos
pid_t init_block(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_kill(int argc, char ** argv, int read_fd, int write_fd, boolean foreground);
pid_t init_nice(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);

// IPC
pid_t init_cat(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_wc(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_filter(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);

// Tests & phylos 
pid_t init_phylo(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_test_sync(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_procs(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_test_prio(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_test_mm(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);




#endif
