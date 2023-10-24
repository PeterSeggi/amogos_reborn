#include "include/userlib.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!\n";
    char buffer_space[1];

    print(test);

    while(1){
        read(buffer_space, 1);
    }
    return 0;
}


int returns_48(){
    return 48;
}
