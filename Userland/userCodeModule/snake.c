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
#define MIN_DIM 5//contempla dim_snake_start=3 con 2 de margen
#define START_MARGIN 2

uint8_t BOARD_H=20; //basandonos en las dimensiones del de maps
uint8_t BOARD_W=20;

uint8_t board[20][20];//valor grande para inicializarla (evitamos tener que usar malloc)

uint8_t fontSize, drawSize;
uint16_t screenHeight, screenWidth;

uint8_t dibSpaceHeight, dibSpaceWidth;//el valor que realmente toma luego del size

uint8_t board_start_x, board_start_y;

uint8_t middleBoard_y;

uint8_t initGame(){
    getScreenData(&screenHeight,&screenWidth,&fontSize,&drawSize);
    dibSpaceHeight=(dibHeight*drawSize);
    dibSpaceWidth=(dibWidth*drawSize);
    BOARD_H= BOARD_H>(screenHeight/dibSpaceHeight)?  (screenHeight/dibSpaceHeight) : BOARD_H;    //buscamos la menor dimension
    BOARD_W= BOARD_W>(screenWidth/dibSpaceWidth)?  (screenWidth/dibSpaceWidth) : BOARD_W;
    BOARD_H>BOARD_W? (BOARD_H=BOARD_W) : (BOARD_W=BOARD_H);//nos aseguramos de que el tablero sea cuadrado
    if(BOARD_H<MIN_DIM) return 1;
    for(int i=0; i<BOARD_W; i++){
        for(int j=0; j<BOARD_H; j++){
            board[i][j]=0;
        }
    }
    board_start_x=(screenWidth-(BOARD_W*dibSpaceWidth))/2;
    board_start_y=(screenHeight-(BOARD_H*dibSpaceHeight))/2;
    middleBoard_y=BOARD_H/2;
    return 0;
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
    board[column][row]=1;
}

void addSnake(uint8_t row, uint8_t column, uint8_t elem){
    board[column][row]=elem;
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

//snake stuff
//head extremo izq, tail extremo der
/*uint8_t snake1_head_row=0;
uint8_t snake1_head_column=middleBoard_y;
uint8_t snake1_tail_row=0;
uint8_t snake1_tail_column=middleBoard_y;

uint8_t snake2_head_row=0;
uint8_t snake2_head_column=middleBoard_y;
uint8_t snake2_tail_row=0;
uint8_t snake2_tail_column=middleBoard_y;*/

//================================================================================================================================
// Main Stuff
//================================================================================================================================
//================================================================================================================================
#define EXIT_KEY 'q'

uint8_t exit=0;//only changes when exit key is pressed to exit the game
uint8_t snakes=0;//quantity of players
#define buffer_dim 1
char keypressed[buffer_dim]={0};


void Snake(){
    if(initGame()){
        print("size to small!");
        return;
    }

    snakeSetup(SNAKE1);    

    while(!exit){
        if(read(keypressed, 1)>0){
            switch(keypressed[0]){
                case ('q'):
                    exit=1;
                    keypressed[0]=0;
                    tablero();
                    break;
                
                case ('a'):
                    break;

                case ('s'):
                    break;

                case ('d'):
                    break;

                case ('w'):
                    break;

                default:
                    break;
            }
        }
        miliSleep(100);
    }

    print("BYE!");

    return;
}

//setup

void snakeSetup(uint8_t snake){
    uint8_t snake_column_start=START_MARGIN;
    if(snakes) snake_column_start=BOARD_W - (MIN_DIM*dibSpaceWidth);

    addSnake(middleBoard_y,snake_column_start,snake);//head
    addSnake(middleBoard_y,snake_column_start+1,snake);//midle
    addSnake(middleBoard_y,snake_column_start+2,snake);//tail

    snakes++;

    tablero();//print the starting board
}