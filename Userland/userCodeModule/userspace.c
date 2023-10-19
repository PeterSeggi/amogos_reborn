#include "include/userlib.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!";
    char* test_error = "Oh no! Emergency meeting called";

    printf(test);
    printError(test_error);

    while(1);
    return 0;
}


int returns_48(){
    return 48;
}
