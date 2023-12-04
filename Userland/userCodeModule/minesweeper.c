#include "include/minesweeper.h"
#include "include/mine_drawings.h"
#include "include/drawings.h"
#include "include/userlib.h"
#include "include/rand.h"

//================================================================================================================================
// Main Screen Stuff
//================================================================================================================================
//================================================================================================================================

/*uint8_t minesweeperScreen(){
    //putMinesweeperTitle();

    #define BEGINNER 0
    #define INTERMEDIATE 1
    #define EXPERT 2

    char keypressed[1]={0};
    uint8_t selected=0, selection=BEGINNER;

    while(!selected){
        if(read(keypressed,1)>0){
            switch(keypressed[0]){
                case('\n'):
                    selected=1;
                    break;

                case('w'):
                case('i'):
                    selection=BEGINNER;
                    break;

                case('s'):
                case('k'):
                    selection=INTERMEDIATE;
                    break;

                default:
                    break;
            }
        }
        //cambiamos  la apariencia segun la selection
        //selectHover(selection); TIENE QUE SER UNO DISTINTO
    }

    return selection;
}*/

//================================================================================================================================
// Board Stuff
//================================================================================================================================
//================================================================================================================================

uint8_t BOARD1_H=8; //beginner as default
uint8_t BOARD1_W=8;

uint8_t board1[20][20];//valor grande para inicializarla (evitamos tener que usar malloc)

uint8_t fontSize1, drawSize1;
uint16_t screenHeight1, screenWidth1;

uint8_t dibSpaceHeight1, dibSpaceWidth1;//el valor que realmente toma luego del size

uint8_t dibHeight1=16,dibWidth1=16;

//================================================================================================================================
// Main Stuff
//================================================================================================================================
//================================================================================================================================
void Minesweeper(){
    //minesweeperScreen();

    changeDrawSize(2);
    getScreenData(&screenHeight1,&screenWidth1,&fontSize1,&drawSize1);
    dibSpaceHeight1=(dibHeight1*drawSize1);
    dibSpaceWidth1=(dibWidth1*drawSize1);

    for(int i=0; i<((screenWidth1/dibSpaceWidth1) +1); i++){
        for(int j=0; j<((screenHeight1/dibSpaceHeight1) +1); j++){
            draw(square[0], 0x00FFF3A5, dibHeight1,i*(dibSpaceWidth1),j*(dibSpaceHeight1));
        }
    }

    draw_box(0,0);
    draw_flag(0,0);
    draw_pressed_box(dibSpaceWidth1,0);
    draw_pressed_box(dibSpaceWidth1*2,0);
    draw_bomb(dibSpaceWidth1*2,0);
    draw_box(dibSpaceWidth1*3,0);

}