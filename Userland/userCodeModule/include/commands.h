#ifndef COMMANDS_H
#define COMMANDS_H

#include "sh.h"
#include "userlibasm.h"
#include "userlib.h"
#include <stdint.h>

#define BUFFER_SIZE 128
#define DEFAULT_PRIORITY 4


pid_t init_ps(int read_fd, int write_fd, boolean foreground);
pid_t init_loop(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_mem(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_block(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_kill(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_nice(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_block(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_cat(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_wc(int argc, char * argv[], int read_fd, int write_fd, boolean foreground);
pid_t init_filter(int argc, char * argv[BUFFER_SIZE], int read_fd, int write_fd, boolean foreground);




#endif