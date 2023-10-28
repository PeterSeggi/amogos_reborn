#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"


int main(){
    // esto se core

    char* greeting = "Hello amogOS!\n";
    print(greeting);
    char buffer_space[1];

    uint32_t colors[]={0x00FAE425,0x00414141,0x00FBFBFB,0x000000};
    draw_snakehead_left(colors,200,0);
    addApple(2,1);
    tablero();

    while(1){
        if (read(buffer_space, 1) == 1){
            print(buffer_space);
        }
    }
    return 0;
}
