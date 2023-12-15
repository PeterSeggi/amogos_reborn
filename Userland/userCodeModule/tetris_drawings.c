#include "include/tetris_drawings.h"

//================================================================================================================================
//bitmaps

uint16_t squareT[][squareHeight]={
    {0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111,
     0b1111111111111111}
};

//================================================================================================================================
//functions
#define GAMESCREEN_W 10
#define GAMESCREEN_H 20

uint64_t gamescreen_x_init, gamescreen_y_init;
uint64_t stats_x_init, stats_y_init;
uint64_t score_x_init, score_y_init;

void draw_tetris_board(){
    uint8_t fontSize, drawSize;
    uint16_t screenHeight, screenWidth;
    uint8_t squareSpaceHeight, squareSpaceWidth;

    changeDrawSize(2);
    getScreenData(&screenHeight,&screenWidth,&fontSize,&drawSize);
    squareSpaceHeight=(squareHeight*drawSize);
    squareSpaceWidth=(squareWidth*drawSize);

    set_init_values(screenHeight,screenWidth,squareSpaceHeight,squareSpaceWidth);

    //background figures
    //TODO redefinir colores con define en vez de hardcodeados
    for(int i=0; i<((screenWidth/squareSpaceWidth) +1); i++){
        for(int j=0; j<((screenHeight/squareSpaceHeight) +1); j++){
            draw(squareT[0], 0x009B979B, squareHeight,i*(squareSpaceWidth),j*(squareSpaceHeight));
        }
    }

    //gamescreen
    for(int i =0; i<GAMESCREEN_W; i++){
        for(int j=0; j<GAMESCREEN_H;j++){
            draw(squareT[0], 0x00000000, squareHeight,gamescreen_x_init+i*(squareSpaceWidth),gamescreen_y_init + j*(squareSpaceHeight));
        }
    }
}

void set_init_values(uint16_t screenHeight, uint16_t screenWidth, uint8_t squareSpaceHeight, uint8_t squareSpaceWidth){
    gamescreen_x_init=(((screenWidth/squareSpaceWidth)-GAMESCREEN_W)/2)*squareSpaceWidth;
    gamescreen_y_init=squareSpaceHeight*2;
}