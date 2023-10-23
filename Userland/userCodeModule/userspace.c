#include "include/userlib.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!\n";
    char* test_error = "Oh no! Emergency meeting called";
    char buffer_space[50];

    print(test);
    printError(test_error);
    read(buffer_space, 5);

    while(1);
    return 0;
}


int returns_48(){
    return 48;
}
