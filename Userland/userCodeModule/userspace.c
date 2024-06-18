// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/rand.h"
#include "include/userlib.h"
#include <stdint.h>


// proc init (allegedly)
int main(){    
    int pipe_init_shell[2] = {0};
    int pipe_shell_init[2] = {0};
    
    while(1){

        if(pipe(pipe_init_shell)) return -1; 
        if(pipe(pipe_shell_init)) return -1; 

        int shell_pid = init_sh(pipe_init_shell[0], pipe_shell_init[1]); 

        if (shell_pid != -1) waitpid(shell_pid);


        pclose(pipe_init_shell[0]);
        pclose(pipe_init_shell[1]);
        pclose(pipe_shell_init[0]);
        pclose(pipe_shell_init[1]);
    }

    return 0;
}
