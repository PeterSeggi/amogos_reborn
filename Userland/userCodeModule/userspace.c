#include "include/userlib.h"
#include "include/shell.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* greeting = "Hello amogOS!\n";
    char buffer_space[1];

    print(greeting);

    while(1){
        if (readRaw(buffer_space, 1) == 1){
//            printHex(buffer_space[0]);
            print("-");
        } 
        halt(); 
    }

    print("Bye now!");

    return 0;
}


int returns_48(){
    return 48;
}
