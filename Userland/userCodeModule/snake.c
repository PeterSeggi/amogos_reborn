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
    board[column][row]=elem + dir;
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
uint8_t snake1_tail_row=0, snake1_tail_column=0;
uint8_t snake2_tail_row=0, snake2_tail_column=0;

uint8_t slither(uint8_t column, uint8_t row, enum Direction dir, uint8_t snake){
    uint8_t newHeadCol=column, newHeadRow=row;
    switch(dir){
        case (UP):
            if(row<=0) return 1;//border colision
            newHeadRow--;
            break;

        case (DOWN):
            if(row>=BOARD_H-1) return 1;//border colision
            newHeadRow++;
            break;

        case (RIGHT):
            if(column>=BOARD_W-1) return 1;//border colision
            newHeadCol++;
            break;

        case (LEFT):
            if(column<=0) return 1;//border colision
            newHeadCol--;
            break;

        default:
            break;
    }
    if((board[newHeadCol][newHeadRow] & 0x0F)==((snake==SNAKE1)? SNAKE2:SNAKE1)) return 2;//snakes colision
    if((board[newHeadCol][newHeadRow] & 0x0F)==1){//apple
        //puntaje++;
    }
    board[newHeadCol][newHeadRow]=snake + dir;
    saveHeadPosition(snake,newHeadCol,newHeadRow);
    changePosition(column,row,dir,snake);
    return 0;
}

uint8_t changePosition(uint8_t column, uint8_t row, enum Direction dir, uint8_t snake){
    if(column==getSnakeTailCol(snake) && row==getSnakeTailRow(snake)){//caso base
        board[column][row]=0;
        return 1;//aviso que debe establecerse nuevas posiciones de tail
    }

    uint8_t prevCol=column, prevRow=row;
    enum Direction currentDir = board[column][row] & 0xF0;
    switch(currentDir){
        case (UP):
            prevRow++;
            break;

        case (DOWN):
            prevRow--;
            break;

        case (RIGHT):
            prevCol--;
            break;

        case (LEFT):
            prevCol++;
            break;

        default:
            break;
    }
    if(changePosition(prevCol,prevRow,currentDir,snake)){
        saveTailPosition(snake,column,row);
        board[column][row]=snake+dir;
    }
    return 0;
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
        case(7):
        case(11):
        case(13):
        case(14):
        case(15):
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

//boolean
uint8_t checkUp(uint8_t row, uint8_t column, uint8_t value){
    return (row==0)? 0 : ((board[column][row-1] & 0x0F)==value);
}

uint8_t checkDown(uint8_t row, uint8_t column, uint8_t value){
    return (row==BOARD_H-1)? 0 : ((board[column][row+1] & 0x0F)==value);
}

uint8_t checkLeft(uint8_t row, uint8_t column, uint8_t value){
    return (column==0)? 0 : ((board[column-1][row] & 0x0F)==value);
}

uint8_t checkRight(uint8_t row, uint8_t column, uint8_t value){
    return (column==BOARD_W-1)? 0 : ((board[column+1][row] & 0x0F)==value);
}

//================================================================================================================================
// Main Stuff
//================================================================================================================================
//================================================================================================================================
#define EXIT_KEY 'q'

void Snake(){
    uint8_t exit=0,error=0;//only changes when exit key is pressed to exit the game or a colision happens
    uint8_t snakes=0;//quantity of players

    char keypressed[1]={0};

    enum Direction lastdir=NONE;

    if(initGame()){
        print("size to small!");
        return;
    }

    snakeSetup(SNAKE1,&snakes);    

    while(!exit && !error){
        if(readLast(keypressed, 1)>0){
            print("\n");
            printHex(keypressed[0]);
            print("\n");
            switch(keypressed[0]){
                case ('q'):
                    exit=1;
                    keypressed[0]=0;
                    tablero();
                    break;
                
                case ('a'):
                    if(lastdir!=RIGHT){//illegal dir
                        lastdir= (lastdir==NONE)? RIGHT : LEFT ;
                    }
                    error = slither(snake1_head_column,snake1_head_row,lastdir,SNAKE1);
                    tablero();
                    break;

                case ('s'):
                    if(lastdir!=UP){//illegal dir
                        lastdir=DOWN;
                    }
                    error = slither(snake1_head_column,snake1_head_row,lastdir,SNAKE1);
                    tablero();
                    break;

                case ('d'):
                    if(lastdir!=LEFT){//illegal dir
                        lastdir=RIGHT;
                    }
                    error = slither(snake1_head_column,snake1_head_row,lastdir,SNAKE1);
                    tablero();
                    break;

                case ('w'):
                    if(lastdir!=DOWN){// illegal dir
                        lastdir=UP;
                    }
                    error = slither(snake1_head_column,snake1_head_row,lastdir,SNAKE1);
                    tablero();
                    break;

                default:
                    break;
            }
        }
        else{
            switch(lastdir){
                case(UP):
                    error = slither(snake1_head_column,snake1_head_row,lastdir,SNAKE1);
                    tablero();
                    break;

                case(DOWN):
                    error = slither(snake1_head_column,snake1_head_row,lastdir,SNAKE1);
                    tablero();
                    break;

                case(LEFT):
                    error = slither(snake1_head_column,snake1_head_row,lastdir,SNAKE1);
                    tablero();
                    break;

                case(RIGHT):
                    error = slither(snake1_head_column,snake1_head_row,lastdir,SNAKE1);
                    tablero();
                    break;

                default:
                    break;
            }
        }
        miliSleep(150);
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
    addSnake(middleBoard_y,snake_column_start+2,snake,RIGHT);//head

    saveTailPosition(snake,snake_column_start,middleBoard_y);
    saveHeadPosition(snake,snake_column_start+2,middleBoard_y);    

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
void saveTailPosition(uint8_t snake, uint8_t column, uint8_t row){
    if(snake==SNAKE1){
        snake1_tail_column=column;
        snake1_tail_row=row;
    }
    else{
        snake2_tail_column=column;
        snake2_tail_row=row;
    }
}

uint8_t getSnakeTailCol(uint8_t snake){
    return (snake==SNAKE1)? snake1_tail_column:snake2_tail_column;
}

uint8_t getSnakeTailRow(uint8_t snake){
    return (snake==SNAKE1)? snake1_tail_row:snake2_tail_row;
}

uint8_t getSnakeHeadCol(uint8_t snake){
    return (snake==SNAKE1)? snake1_head_column:snake2_head_column;
}

uint8_t getSnakeHeadRow(uint8_t snake){
    return (snake==SNAKE1)? snake1_head_row:snake2_head_row;
}
