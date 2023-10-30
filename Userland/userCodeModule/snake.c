#include "include/snake.h"
#include "include/drawings.h"
#include "include/userlib.h"

#define APPLE 1
#define SNAKE1  7
#define SNAKE2  14

//================================================================================================================================
// Board Stuff
//================================================================================================================================
//================================================================================================================================

uint8_t BOARD_H=20; //basandonos en las dimensiones del de maps
uint8_t BOARD_W=20;

uint8_t board[20][20];//valor grande para inicializarla (evitamos tener que usar malloc)

uint8_t fontSize, drawSize;
uint16_t screenHeight, screenWidth;

uint8_t dibSpaceHeight, dibSpaceWidth;//el valor que realmente toma luego del size

uint8_t board_start_x, board_start_y;

void initGame(){
    getScreenData(&screenHeight,&screenWidth,&fontSize,&drawSize);
    dibSpaceHeight=(dibHeight*drawSize);
    dibSpaceWidth=(dibWidth*drawSize);
    BOARD_H= BOARD_H>(screenHeight/dibSpaceHeight)?  (screenHeight/dibSpaceHeight) : BOARD_H;    //buscamos la menor dimension
    BOARD_W= BOARD_W>(screenWidth/dibSpaceWidth)?  (screenWidth/dibSpaceWidth) : BOARD_W;
    BOARD_H>BOARD_W? (BOARD_H=BOARD_W) : (BOARD_W=BOARD_H);//nos aseguramos de que el tablero sea cuadrado
    for(int i=0; i<BOARD_W; i++){
        for(int j=0; j<BOARD_H; j++){
            board[i][j]=0;
        }
    }
    board_start_x=(screenWidth-(BOARD_W*dibSpaceWidth))/2;
    board_start_y=(screenHeight-(BOARD_H*dibSpaceHeight))/2;
}

void tablero(){
    
    for(uint8_t i=0; i<BOARD_W; i++){
        for(uint8_t j=0; j<BOARD_H;j++){
            //dibujo la casilla
            uint32_t color= 
                (( ((i%(2)==0) && (j%(2))) ||  ((i%(2)) && (j%(2)==0))))? BOARDCOLOR1:BOARDCOLOR2;
            draw(square[0], color, dibHeight, i*(dibSpaceWidth) + board_start_x, j*(dibSpaceHeight) + board_start_y);

            uint8_t casilla = board[i][j];
            if(casilla==APPLE) draw_manzana(i*(dibSpaceWidth) + board_start_x ,j*(dibSpaceHeight) + board_start_y);
            else if(casilla==SNAKE1 || casilla==SNAKE2) putSnake(j,i,casilla);
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
            draw_snakehead_left(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
            break;
        case(2):
            draw_snakehead_right(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
            break;
        case(3):
            draw_snakebody_horizontal(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
            break;
        case(4):
            draw_snakehead_up(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
            break;
        case(5):
            draw_snakecurve_downleft(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
            break;
        case(6):
            draw_snakecurve_downright(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
            break;
        case(8):
            draw_snakehead_down(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
            break;
        case(9):
            draw_snakecurve_upleft(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
            break;
        case(10):
            draw_snakecurve_upright(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
            break;
        case(12):
            draw_snakebody_vertical(column*(dibSpaceWidth) + board_start_x ,row*(dibSpaceHeight) + board_start_y);
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

//================================================================================================================================
// Logic Stuff
//================================================================================================================================
//================================================================================================================================
//players stuff
uint16_t player1Points=0;
uint16_t player2Points=0;

//================================================================================================================================
// Main Stuff
//================================================================================================================================
//================================================================================================================================
#define EXIT_KEY 'q'

char exit=0;//only changes when exit key is pressed to exit the game
#define buffer_dim 18
char keypressed[buffer_dim];


void Snake(){
    initGame();

    addSnake(6,1,7);
    addSnake(5,1,7);
    addSnake(5,2,7);
    addSnake(4,2,7);

    addSnake(2,3,7);
    addSnake(2,4,7);
    addSnake(3,4,7);
    addSnake(3,5,7);

    while(!exit){
        if(read(keypressed, 1) == 1){
            for(int i=0; !exit && i<buffer_dim; i++){
                if(keypressed[i]==EXIT_KEY) exit=1;
                keypressed[i]=0;
            }
        }
        tablero();
        miliSleep(50);
    }

    print("BYE!");

    return;
}