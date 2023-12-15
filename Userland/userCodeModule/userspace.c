#include "include/userlib.h"
#include "include/shell.h"
#include "include/userlibasm.h"
//#include "include/drawings.h" //multiple definitions of square otherwise
//TODO revisar tema de lo bitmaps extern que sino va a ser dolor de cabeza
#include "include/snake.h"
#include "include/minesweeper.h"
#include "include/tetris.h"
#include "include/rand.h"


int main(){    

    Tetris();
    //Minesweeper();
    /*
    while(1){
        uint8_t selection = mainMenu();

        if(selection==1){
            shell();
        }
        else{
            Snake();
        }
    }*/

    return 0;
}
