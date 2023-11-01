#include "include/userlib.h"
#include "include/shell.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* greeting = "Hello amogOS!\n";
    char buffer_space[1];

    print(greeting);

    shell();

    print("Bye now!");

    return 0;
}
