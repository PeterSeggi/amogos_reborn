#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"


int main(){
    // esto se core

    char* greeting = "Hello amogOS!\n";
    print(greeting);
    char buffer_space[1];

    Snake();
/*
    while(1){
        if (read(buffer_space, 1) == 1){
            print(buffer_space);
        }
    }
*/
    return 0;
}
