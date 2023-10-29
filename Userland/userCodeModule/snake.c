#include "include/snake.h"
#include "include/drawings.h"
#include "include/userlib.h"

#define SNAKE1  7
#define SNAKE2  14

uint8_t BOARD_H;
uint8_t BOARD_W;

uint8_t board[1000][1000];//valor grande para inicializarla (evitamos tener que usar malloc)

uint8_t fontSize, drawSize;
uint16_t screenHeight, screenWidth;

void initGame(){
    getScreenData(&screenHeight,&screenWidth,&fontSize,&drawSize);
    BOARD_H=screenHeight/dibHeight;
    BOARD_W=screenWidth/dibWidth;
    for(int i=0; i<BOARD_W; i++){
        for(int j=0; j<BOARD_H; j++){
            board[i][j]=0;
        }
    }
}

//================================================================================================================================
// Tablero de juego
//================================================================================================================================
//================================================================================================================================
//logistica: casilleros vacios almacenan 0, las manzanas suman 1.
//identificamos que en un casillero una snake comio manzana ya que su contador incremento solo en 1
//identificamos choques entre snakes ya que la suma del casillero va a dar un numero mucho mas alto
//snake 1= 7;  snake 2= 14;
void tablero(){
    
    for(uint8_t i=0; i<BOARD_W; i++){
        for(uint8_t j=0; j<BOARD_H;j++){
            //dibujo la casilla
            uint32_t color= 
                (( ((i%(2)==0) && (j%(2))) ||  ((i%(2)) && (j%(2)==0))))? BOARDCOLOR1:BOARDCOLOR2;
            draw(square[0], color, dibHeight, i*(dibWidth*drawSize),j*(dibHeight*drawSize));

            uint8_t casilla = board[i][j];
            switch(casilla){
                case(0)://no hay nada
                    break;
                case(1)://manzana
                    draw_manzana(i*(dibWidth*drawSize),j*(dibHeight*drawSize));
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

void addSnake(uint8_t row, uint8_t column, uint8_t elem){
    board[column-1][row-1]=elem;
}

void putSnake(uint8_t row, uint8_t column, uint8_t snake){//contemplamos casos de que parte de la serpiente imprimir
    uint8_t caso = checkRight(row,column,snake);
    caso += checkLeft(row,column,snake)<<1;
    caso += checkDown(row,column,snake)<<2;
    caso += checkUp(row,column,snake)<<3;
    switch(caso){
        case(1):
            draw_snakehead_left(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
            break;
        case(2):
            draw_snakehead_right(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
            break;
        case(3):
            draw_snakebody_horizontal(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
            break;
        case(4):
            draw_snakehead_up(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
            break;
        case(5):
            draw_snakecurve_downleft(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
            break;
        case(6):
            draw_snakecurve_downright(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
            break;
        case(8):
            draw_snakehead_down(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
            break;
        case(9):
            draw_snakecurve_upleft(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
            break;
        case(10):
            draw_snakecurve_upright(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
            break;
        case(12):
            draw_snakebody_vertical(column*(dibWidth*drawSize),row*(dibHeight*drawSize));
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