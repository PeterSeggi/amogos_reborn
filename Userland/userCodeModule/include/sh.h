#ifndef SH_H 
#define SH_H 

#define BUFFER_SIZE 128
#define MAX_PROCESS_COUNT 100

int sh();
int init_sh(int read_fd, int write_fd);
void process_command();
void process(char key);
void parse_command(char *input, char *c1, char **a1, int *a1_size);

#endif


