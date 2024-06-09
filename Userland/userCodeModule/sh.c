#include "include/userlib.h"
#include "include/sh.h"
#include "include/shell.h"
#include "include/userlibasm.h"
#include "include/snake.h"
#include <stdint.h>
#include <stddef.h>

int init_sh(){
    return create_process(&sh);
}

int sh(){

    clearScreen();
    while(1){

    }
    /*
    init_shell();
    write_out(PROMPT_START);


    while(!exit_command){
        if (read(char_buffer, 1) == 1){
            process_key(char_buffer[0]);
        }
        //sleep_once();
    }

    clearScreen();

    */
    return 0;
}


