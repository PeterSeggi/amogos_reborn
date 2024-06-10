#ifndef SH_H 
#define SH_H 

#define BUFFER_SIZE 128

int sh();
int init_sh();
void process_command();
void process(char key);
void parse_command(const char *input, char *c1, char **a1, int *a1_size);

#endif


