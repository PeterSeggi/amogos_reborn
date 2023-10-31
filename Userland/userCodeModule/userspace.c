#include "include/userlib.h"
#include "include/shell.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"


int main(){
    // esto se core

    char* greeting = "Hello amogOS!\n";
    print(greeting);
    char buffer_space[1];

    //Snake();
    while(1){
        if (readLast(buffer_space, 1) == 1){
            printDec(buffer_space[0]);
            print("-");
        }
        miliSleep(500);
    }
    return 0;
}
