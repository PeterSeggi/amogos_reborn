#ifndef COMMANDS_H
#define COMMANDS_H

#include "sh.h"
#include "userlibasm.h"
#include "userlib.h"
#include <stdint.h>

#define DEFAULT_PRIORITY 4


pid_t init_ps(int read_fd, int write_fd, boolean foreground);
pid_t init_loop(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_mem(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_malloc(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_free(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_block(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_pipe(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);


#endif