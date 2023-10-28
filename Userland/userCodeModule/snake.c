#include "include/snake.h"
#include "include/drawings.h"
#include "include/userlib.h"

#define BOARD_H 6
#define BOARD_W 6

uint8_t board[BOARD_H][BOARD_W]={0};

void tablero(){
    //uint8_t size=_getDrawSize();
    uint8_t size=3;
    for(int i=0; i<BOARD_W; i++){
        for(int j=0; j<BOARD_H;j++){
            //dibujo la casilla
            uint32_t color= 
                (( ((i%(2)==0) && (j%(2))) ||  ((i%(2)) && (j%(2)==0))))? BOARDCOLOR1:BOARDCOLOR2;
            draw(square[0], color, dibHeight, i*(dibWidth*size),j*(dibHeight*size));

            uint8_t casilla = board[i][j];
            switch(casilla){
                case(0)://no hay nada
                    break;
                case(1)://manzana
                    draw_manzana(i*(dibWidth*size),j*(dibHeight*size));
                    break;
                default://casos de snakes
                    break;
            }
        }
    }
}

void addApple(uint8_t row, uint8_t column){
    board[column-1][row-1]=1;//resta menos uno para contemplar que uno no piensa arrancando en cero jajj
}