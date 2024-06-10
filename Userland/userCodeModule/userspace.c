
#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"
#include "include/rand.h"
#include "include/userlib.h"
#include <stdint.h>


// proc init (allegedly)
int main(){    
    
    while(1){
        int pipe_init_shell[2] = {0};
        int pipe_shell_init[2] = {0};

        if(pipe(pipe_init_shell)) return -1; 
        if(pipe(pipe_shell_init)) return -1; 

        int shell_pid = init_sh(pipe_init_shell[1], pipe_shell_init[0]); 

        
        waitpid(shell_pid);
    }

    return 0;
}
