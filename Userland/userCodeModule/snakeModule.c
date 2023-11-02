#include "include/snake.h"
#include "include/drawings.h"
#include "include/userlib.h"

#define PLAYER1 0
#define PLAYER2 1


uint8_t snakeScreen(){
    //title
    putSnakeTitle();

    //select
    #define UPSELECT PLAYER1
    #define DOWNSELECT PLAYER2

    char keypressed[1]={0};
    uint8_t selected=0, selection=UPSELECT;

    while(!selected){
        if(readLast(keypressed,1)>0){
            switch(keypressed[0]){
                case('\n'):
                    selected=1;
                    break;

                case('w'):
                case('i'):
                    selection=UPSELECT;
                    break;

                case('s'):
                case('k'):
                    selection=DOWNSELECT;
                    break;

                default:
                    break;
            }
        }
        //cambiamos  la apariencia segun la selection
        selectHover(selection);
    }

    uint32_t c1=0;
    uint32_t c2=0;

    selectColor(selection, &c1, &c2);

    return selection;
}