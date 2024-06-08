#include "include/userlib.h"
#include "include/sh.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"
#include "include/rand.h"
#include <stdint.h>


int main(){    
    
    while(1){
        int (*sh_direc)() = sh;
        int shell_pid = create_process((void *) sh_direc);
        waitpid(shell_pid);
    }

    return 0;
}
