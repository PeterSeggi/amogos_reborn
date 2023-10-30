#include "include/snake.h"
#include "include/drawings.h"
#include "include/userlib.h"

#define APPLE 1
#define SNAKE1  4
#define SNAKE2  8

//================================================================================================================================
// Board Stuff
//================================================================================================================================
//================================================================================================================================
#define MIN_DIM 5//contempla dim_snake_start=3 con 2 de margen
#define START_MARGIN 2

uint8_t BOARD_H=20; //basandonos en las dimensiones del de maps
uint8_t BOARD_W=20;

uint8_t board[20][20];//valor grande para inicializarla (evitamos tener que usar malloc)
//b7-b4 for direction b3-b0 for value
//we use & 0x0F and 0xF0 multiple times to read the corresponding value

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

            uint8_t casilla = board[i][j] & 0x0F;
            if(casilla==APPLE) draw_manzana(i*(dibSpaceWidth) + board_start_x ,j*(dibSpaceHeight) + board_start_y);
            else if(casilla==SNAKE1 || casilla==SNAKE2) putSnake(j,i,casilla);
        }
    }
}

void addApple(uint8_t row, uint8_t column){
    board[column][row]=1;
}

void addSnake(uint8_t row, uint8_t column, uint8_t elem, enum Direction dir){
    print("adding");
    board[column][row]=elem + dir;
    printHex(column);
    printHex(row);
    print("\n");
}



//================================================================================================================================
// Logic Stuff
//================================================================================================================================
//================================================================================================================================
//players stuff
uint16_t player1Points=0;
uint16_t player2Points=0;

//directions stuff

uint8_t snake1_head_row=0, snake1_head_column=0;
uint8_t snake2_head_row=0, snake2_head_column=0;

void mov_snake_down(uint8_t snake){
    print("DOWN");
}

void mov_snake_up(uint8_t snake){
    print("UP");
}

void mov_snake_left(uint8_t snake){
    print("LEFT");
}

void mov_snake_right(uint8_t snake){
    print("RIGHT");
}



void test(uint8_t snake, uint8_t column, uint8_t row, enum Direction dir){
    //checkeamos cual es la siguiente a mover
    uint8_t caso = checkRight(row,column,snake);
    caso += checkLeft(row,column,snake)<<1;
    caso += checkDown(row,column,snake)<<2;
    caso += checkUp(row,column,snake)<<3;
    switch(caso){
        case(1):
            break;
        case(2):
            break;
        case(3):
            break;
        case(4):
            break;
        case(5):
            break;
        case(6):
            break;
        case(8):
            break;
        case(9):
            break;
        case(10):
            break;
        case(12):
            break;
        default:
            break;
    }

    //movemos la seccion del snake
    switch(dir){
        case(UP):
            board[column][row+1]=snake;
            board[column][row]=0;
            break;

        case(DOWN):
            board[column][row-1]=snake;
            board[column][row]=0;
            break;

        case(LEFT):
            board[column-1][row]=snake;
            board[column][row]=0;
            break;

        case(RIGHT):
            board[column+1][row]=snake;
            board[column][row]=0;
            break;

        default:
            break;
    }

    //test(,)
}

void putSnake(uint8_t row, uint8_t column, uint8_t snake){//contemplamos casos de que parte de la serpiente imprimir
    enum Direction currentDir = getDirection(row,column);
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
        case(7):
        case(11):
        case(13):
        case(14):
        case(15):
            print("other\n");
            printHex(caso);
            print("\n");
            checkWithOthersDir(getDirection(row,column),row,column,caso);
            break;
        default://casos sin sentido
            break;
    }
}


//current receives de flow from left(Left goes Right), from right(Right goes Left), from down(Down goes Up) or from up(Up goes Down)

void checkWithOthersDir( enum Direction currenDir, uint8_t currentRow, uint8_t currentCol, uint8_t othersInfo){
    switch(currenDir){
                case(UP):
                    if( (othersInfo & 0x02) && getDirection(currentRow,currentCol-1)==RIGHT)
                        draw_snakecurve_upright(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if( (othersInfo & 0x01) && getDirection(currentRow,currentCol+1)==LEFT)
                        draw_snakecurve_upleft(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if( (othersInfo & 0x04) && getDirection(currentRow-1,currentCol)==UP)
                        draw_snakebody_vertical(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if(!(othersInfo & 0x08))
                        draw_snakehead_up(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    break;
                case(DOWN):
                    if( (othersInfo & 0x02) && getDirection(currentRow,currentCol-1)==RIGHT)
                        draw_snakecurve_downright(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if( (othersInfo & 0x01) && getDirection(currentRow,currentCol+1)==LEFT)
                        draw_snakecurve_downleft(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if(!(othersInfo & 0x04))
                        draw_snakehead_down(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if( (othersInfo & 0x08) && getDirection(currentRow+1,currentCol)==DOWN)
                        draw_snakebody_vertical(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    break;
                case(LEFT):
                    if(!(othersInfo & 0x02))
                        draw_snakehead_left(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if( (othersInfo & 0x01) && getDirection(currentRow,currentCol+1)==LEFT)
                        draw_snakebody_horizontal(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if( (othersInfo & 0x04) && getDirection(currentRow-1,currentCol)==UP)
                        draw_snakecurve_downright(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if( (othersInfo & 0x08) && getDirection(currentRow+1,currentCol)==DOWN)
                        draw_snakecurve_upright(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    break;
                case(RIGHT):
                    if( (othersInfo & 0x02) && getDirection(currentRow,currentCol-1)==RIGHT)
                        draw_snakebody_horizontal(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if(!(othersInfo & 0x01))
                        draw_snakehead_right(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if( (othersInfo & 0x04) && getDirection(currentRow-1,currentCol)==UP)
                        draw_snakecurve_downleft(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    else if( (othersInfo & 0x08) && getDirection(currentRow+1,currentCol)==DOWN)
                        draw_snakecurve_upleft(currentCol*(dibSpaceWidth) + board_start_x ,currentRow*(dibSpaceHeight) + board_start_y);
                    break;
                default:
                    break;
            }
}

//getting info from board
enum Direction getDirection(uint8_t row, uint8_t column){
    uint8_t caso = board[column][row] & 0xF0;//me quedo con los bits superiores, la direccion
    switch(caso){
        case (UP):
            return UP;

        case (DOWN):
            return DOWN;

        case (RIGHT):
            return RIGHT;

        case (LEFT):
            return LEFT;

        default:
            return NONE;
    }
}

//check
uint8_t checkColision(uint8_t column, uint8_t row){
    return (board[column][row]==0) || (board[column][row]==APPLE);
}

//boolean
uint8_t checkUp(uint8_t row, uint8_t column, uint8_t value){
    return row==0? 0 : ((board[column][row-1] & 0x0F)==value);
}

uint8_t checkDown(uint8_t row, uint8_t column, uint8_t value){
    return row==BOARD_H-1? 0 : ((board[column][row+1] & 0x0F)==value);
}

uint8_t checkLeft(uint8_t row, uint8_t column, uint8_t value){
    return column==0? 0 : ((board[column-1][row] & 0x0F)==value);
}

uint8_t checkRight(uint8_t row, uint8_t column, uint8_t value){
    return column==BOARD_W-1? 0 : ((board[column+1][row] & 0x0F)==value);
}

//================================================================================================================================
// Main Stuff
//================================================================================================================================
//================================================================================================================================
#define EXIT_KEY 'q'

void Snake(){
    uint8_t exit=0;//only changes when exit key is pressed to exit the game
    uint8_t snakes=0;//quantity of players

    char keypressed[1]={0};

    enum Direction lastdir=NONE;

    if(initGame()){
        print("size to small!");
        return;
    }

    snakeSetup(SNAKE1,&snakes);    

    while(!exit){
        if(read(keypressed, 1)>0){
            switch(keypressed[0]){
                case ('q'):
                    exit=1;
                    keypressed[0]=0;
                    tablero();
                    break;
                
                case ('a'):
                    lastdir==NONE? mov_snake_right(SNAKE1) : mov_snake_left(SNAKE1);//restriction of illegal first move(based off maps)
                    lastdir= lastdir==NONE? RIGHT : LEFT ;
                    break;

                case ('s'):
                    mov_snake_down(SNAKE1);
                    lastdir=DOWN;
                    break;

                case ('d'):
                    mov_snake_right(SNAKE1);
                    lastdir=RIGHT;
                    break;

                case ('w'):
                    mov_snake_up(SNAKE1);
                    lastdir=UP;
                    break;

                default:
                    break;
            }
        }
        else{
            switch(lastdir){
                case(UP):
                    mov_snake_up(SNAKE1);
                    break;

                case(DOWN):
                    mov_snake_down(SNAKE1);
                    break;

                case(LEFT):
                    mov_snake_left(SNAKE1);
                    break;

                case(RIGHT):
                    mov_snake_right(SNAKE1);
                    break;

                default:
                    break;
            }
        }
        miliSleep(200);
    }

    print("BYE!");

    return;
}

//setup

void snakeSetup(uint8_t snake, uint8_t * snakes){
    uint8_t snake_column_start=START_MARGIN;
    if(*snakes){
        snake_column_start=BOARD_W - (MIN_DIM*dibSpaceWidth);
    }

    addSnake(middleBoard_y,snake_column_start,snake,RIGHT);//tail
    addSnake(middleBoard_y,snake_column_start+1,snake,RIGHT);//midle
    addSnake(middleBoard_y,snake_column_start+2,snake,UP);//head

    saveHeadPosition(snake,snake1_head_column+2,middleBoard_y);
// Esto rompe una falla en mi logica, tengo que ver como resolverlo
    addSnake(middleBoard_y-1,snake_column_start+2,snake,RIGHT);
    addSnake(middleBoard_y-1,snake_column_start+3,snake,DOWN);
    addSnake(middleBoard_y,snake_column_start+3,snake,DOWN);
    addSnake(middleBoard_y+1,snake_column_start+3,snake,DOWN);
    

    *snakes++;

    tablero();//print the starting board
}

void saveHeadPosition(uint8_t snake, uint8_t column, uint8_t row){
    if(snake==SNAKE1){
        snake1_head_column=column;
        snake1_head_row=row;
    }
    else{
        snake2_head_column=column;
        snake2_head_row=row;
    }
}