#ifndef SH_H 
#define SH_H 

#include "userlib.h"

#define BUFFER_SIZE 128
#define MAX_PROCESS_COUNT 100

void sh();
int init_sh(int read_fd, int write_fd);
void process_command();
void process(char key);
int parse_command(char *input, int r_fd, int w_fd, boolean foreground);
void command_wrapper(char* input);

#endif


