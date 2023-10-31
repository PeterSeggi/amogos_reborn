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

    Snake(2,0x00FF71BE,0x00FAE425);
    return 0;
}
