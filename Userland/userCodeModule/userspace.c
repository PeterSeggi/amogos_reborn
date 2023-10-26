#include "include/userlib.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!\n";
    print(test);

    draw();

    return 0;
}


int returns_48(){
    return 48;
}
