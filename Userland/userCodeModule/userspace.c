#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/drawings.h"


int main(){
    // esto se core

    char* greeting = "Hello amogOS!\n";
    print(greeting);
    char buffer_space[1];

    draw(test[0],0x008E85FF,charHeight,0,0);

    while(1){
        if (read(buffer_space, 1) == 1){
            print(buffer_space);
        }
    }
    return 0;
}
