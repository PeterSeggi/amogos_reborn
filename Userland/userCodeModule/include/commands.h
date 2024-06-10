#ifndef COMMANDS_H
#define COMMANDS_H

#include "sh.h"
#include "userlibasm.h"
#include "userlib.h"
#include <stdint.h>


pid_t init_ps();
pid_t init_loop(int argc, char * argv[]);

#endif