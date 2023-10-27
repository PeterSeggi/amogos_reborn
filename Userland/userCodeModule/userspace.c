#include "include/userlib.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!\n";
    int buffer_space[1];

    print(test);

    while(1){
        if (read(buffer_space, 1) == 1){
            print(buffer_space);
        }
    }
    return 0;
}


int returns_48(){
    return 48;
}
