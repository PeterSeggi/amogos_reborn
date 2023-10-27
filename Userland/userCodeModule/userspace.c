#include "include/userlib.h"
#include "include/shell.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!\n";
    char buffer_space[1];

    print(test);

    shell();

    return 0;
}


int returns_48(){
    return 48;
}
