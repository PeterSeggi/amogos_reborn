
#include "include/shell.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"
#include "include/rand.h"
#include "include/userlib.h"
#include <stdint.h>


int main(){    
    
    while(1){
        uint8_t selection = mainMenu();

        if(selection==1){
            pid_t shellPid = the_shell();
            waitpid(shellPid);
        }
        else{
            Snake();
        }
    }

    return 0;
}
