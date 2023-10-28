#include "include/snake.h"
#include "include/drawings.h"
#include "include/userlib.h"

#define BOARD_H 6
#define BOARD_W 6

#define SNAKE1  7
#define SNAKE2  14

uint8_t board[BOARD_H][BOARD_W]={0};

//================================================================================================================================
// Tablero de juego
//================================================================================================================================
//================================================================================================================================
//logistica: casilleros vacios almacenan 0, las manzanas suman 1.
//identificamos que en un casillero una snake comio manzana ya que su contador incremento solo en 1
//identificamos choques entre snakes ya que la suma del casillero va a dar un numero mucho mas alto
//snake 1= 7;  snake 2= 14;
void tablero(){
    //uint8_t size=_getDrawSize();
    uint8_t size=3;
    for(uint8_t i=0; i<BOARD_W; i++){
        for(uint8_t j=0; j<BOARD_H;j++){
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
                case(7)://snake1
                    putSnake(j,i,casilla);
                    break;
                case(8)://snake1 comio manzana
                    break;
                case(14)://snake2
                    break;
                case(15)://snake2 comio manzana
                    break;
                default://casos de colision entre serpientes(la logica no es en esta parte, no deberia ocurrir)
                    break;
            }
        }
    }
}

void addApple(uint8_t row, uint8_t column){
    board[column-1][row-1]=1;//resta menos uno para contemplar que uno no piensa arrancando en cero jajj
}

void putSnake(uint8_t row, uint8_t column, uint8_t snake){//contemplamos casos de que parte de la serpiente imprimir
    uint8_t caso = checkRight(row,column,snake);
    caso += checkLeft(row,column,snake)<<1;
    caso += checkDown(row,column,snake)<<2;
    caso += checkUp(row,column,snake)<<3;
    switch(caso){
        case(1):
            //draw head left
            break;
        case(2):
            //draw head right
            break;
        case(3):
            //draw body horizontal
            break;
        case(4):
            //draw head up
            break;
        case(5):
            //draw curve down-to-right
            break;
        case(6):
            //draw curve left-to-down
            break;
        case(8):
            //draw head down
            break;
        case(9):
            //draw curve up-to-rght
            break;
        case(10):
            //draw curve up-to-left
            break;
        case(12):
            //draw body vertical
            break;
        default://casos sin sentido
            break;
    }
}

//boolean
uint8_t checkUp(uint8_t row, uint8_t column, uint8_t value){
    return row==0? 0 : (board[column][row-1]==value);
}

uint8_t checkDown(uint8_t row, uint8_t column, uint8_t value){
    return row==BOARD_H-1? 0 : (board[column][row+1]==value);
}

uint8_t checkLeft(uint8_t row, uint8_t column, uint8_t value){
    return column==0? 0 : (board[column-1][row]==value);
}

uint8_t checkRight(uint8_t row, uint8_t column, uint8_t value){
    return column==BOARD_W-1? 0 : (board[column+1][row]==value);
}