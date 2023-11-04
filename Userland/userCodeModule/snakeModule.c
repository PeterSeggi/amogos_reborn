#include "include/snake.h"
#include "include/drawings.h"
#include "include/userlib.h"

#define PLAYER1 0
#define PLAYER2 1


uint8_t snakeScreen(){
    //title
    putSnakeTitle();

    #define PLAYERS_1 0
    #define PLAYERS_2 1

    char keypressed[1]={0};
    uint8_t selected=0, selection=PLAYERS_1;

    while(!selected){
        if(read(keypressed,1)>0){
            switch(keypressed[0]){
                case('\n'):
                    selected=1;
                    break;

                case('w'):
                case('i'):
                    selection=PLAYERS_1;
                    break;

                case('s'):
                case('k'):
                    selection=PLAYERS_2;
                    break;

                default:
                    break;
            }
        }
        //cambiamos  la apariencia segun la selection
        selectHover(selection);
    }

    return selection;
}