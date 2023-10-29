#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"


int main(){
    // esto se core

    char* greeting = "Hello amogOS!\n";
    print(greeting);
    char buffer_space[1];

    addSnake(3,1,7);
    addSnake(3,2,7);
    addSnake(3,3,7);
    addSnake(3,4,7);
    addSnake(3,5,7);
    tablero();

    while(1){
        if (read(buffer_space, 1) == 1){
            print(buffer_space);
        }
    }
    return 0;
}
