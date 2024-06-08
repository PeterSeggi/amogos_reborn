#ifndef SHELL_H
#define SHELL_H

"include/userlib.h"

pid_t the_shell();
int shell();
void process_key(char key);
void process_command(char* buffer);
void shift();
int check_shift();
void write_out(char* string);
void init_shell();
void resize();
void desize();

#endif