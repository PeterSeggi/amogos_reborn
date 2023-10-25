#include "include/userlib.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!\n";
    char buffer_space[1];

    print(test);
    printTime();
    nanosleep(3, 1);
    printTime();

    while(1){
        if (read(buffer_space, 1) == 1){
            if (buffer_space[0] == 0x1E)
                print("Boca ");
        }
    }
    return 0;
}


int returns_48(){
    return 48;
}
