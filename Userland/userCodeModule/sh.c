#include "include/sh.h"
#include "include/userlib.h"
#include <stdint.h>

int init_sh(){
    return create_process(&sh);
}

char* let = " ";

int sh(){

    clearScreen();
    while(1){
        if (read(let, 1) == 1){
            print(let);
        }

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


