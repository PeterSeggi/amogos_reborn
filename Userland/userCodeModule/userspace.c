#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"


int main(){
    // esto se core

    char* greeting = "Hello amogOS!\n";
    print(greeting);
    char buffer_space[1];

    addSnake(6,1,7);
    addSnake(5,1,7);
    addSnake(5,2,7);
    addSnake(4,2,7);

    addSnake(2,3,7);
    addSnake(2,4,7);
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
