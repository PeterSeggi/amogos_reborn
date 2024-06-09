#include "include/userlib.h"
#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"
#include "include/rand.h"
#include <stdint.h>


// proc init (allegedly)
int main(){    
    
    while(1){
        int shell_pid = init_sh(); 
        waitpid(shell_pid);
    }

    return 0;
}
