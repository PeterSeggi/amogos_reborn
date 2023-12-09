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

    for(int i =0; i<BOARD1_W; i++){
        for(int j=0; j<BOARD1_H;j++){
            draw_box(dibSpaceWidth1*i,dibSpaceHeight1*j);
        }
    }

    for(int i=0; i<10; i++){//difficulty begginer places 10 bombs
        place_bomb();
    }

    sleep(5,1);

    for(int i =0; i<BOARD1_H; i++){
        for(int j=0; j<BOARD1_W;j++){
            if(board1[j][i]==1) draw_bomb(dibSpaceWidth1*i,dibSpaceHeight1*j);
            else draw_proximity(dibSpaceWidth1*i,dibSpaceHeight1*j,get_bombs(j,i));
        }
    }

    /*
    draw_flag(0,0);
    draw_pressed_box(dibSpaceWidth1,0);
    draw_pressed_box(dibSpaceWidth1*2,0);
    draw_bomb(dibSpaceWidth1*2,0);
    draw_box(dibSpaceWidth1*3,0);
    */

}

void place_bomb(){
    uint8_t row, column;
    do{
        row = (uint8_t) (rand()%BOARD1_H);
        column = (uint8_t) (rand()%BOARD1_W);
    }
    while(board1[column][row]);//espero a una casilla no ocupada
    board1[column][row]=1;//bomba
}

uint8_t get_bombs(uint8_t column, uint8_t row){
    //unchecked values. they should be within the board dimensions.
    uint8_t toRet=0;
    for(int i=-1; i<2; i++){
        if(column+i>=0 && column+i<BOARD1_W){
            if(row>0) toRet+=board1[column+i][row-1];
            toRet+=board1[column+i][row+0];
            if(row<BOARD1_H) toRet+=board1[column+i][row+1];
        }
    }
    return toRet;
}