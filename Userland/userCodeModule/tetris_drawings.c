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

#define TETRIS_FIGURE_SQUARE_LAYERS 2
uint32_t tetris_figure_square_colors[TETRIS_FIGURE_SQUARE_LAYERS]={0x00FBFBFB,0x00000000};
uint16_t tetrisFigureSquare[][squareHeight]={

//color
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
     0b1111111111111111},

//white
    {0b1111111111111111,
     0b1111111111111111,
     0b1111000000000011,
     0b1111000000000011,
     0b1100111100000011,
     0b1100111100000011,
     0b1100110000000011,
     0b1100110000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1111111111111111,
     0b1111111111111111},

    //border
    {0b1111111111111111,
     0b1111111111111111,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1100000000000011,
     0b1111111111111111,
     0b1111111111111111},
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

    squareSpaceHeight/=2;
    squareSpaceWidth/=2;

    draw_tetris_block(gamescreen_x_init+squareSpaceWidth*3,gamescreen_y_init+squareSpaceHeight*3,0x00FF0000);
    draw_tetris_block(gamescreen_x_init+squareSpaceWidth*4,gamescreen_y_init+squareSpaceHeight*3,0x00FF0000);
    draw_tetris_block(gamescreen_x_init+squareSpaceWidth*3,gamescreen_y_init+squareSpaceHeight*4,0x00FF0000);
    draw_tetris_block(gamescreen_x_init+squareSpaceWidth*2,gamescreen_y_init+squareSpaceHeight*4,0x00FF0000);

}

void set_init_values(uint16_t screenHeight, uint16_t screenWidth, uint8_t squareSpaceHeight, uint8_t squareSpaceWidth){
    gamescreen_x_init=(((screenWidth/squareSpaceWidth)-GAMESCREEN_W)/2)*squareSpaceWidth;
    gamescreen_y_init=squareSpaceHeight*2;
}

void draw_tetris_block(uint64_t init_x, uint64_t init_y,uint32_t block_color){
    draw_block(tetrisFigureSquare, TETRIS_FIGURE_SQUARE_LAYERS, tetris_figure_square_colors,init_x,init_y,block_color);
}

void draw_block(uint16_t bitmap[][squareHeight], uint16_t layers, uint32_t colors[], uint64_t init_x, uint64_t init_y, uint32_t block_color){
    changeDrawSize(1);
    for(int i=0; i<layers+1; i++){
        if(i==0) draw(bitmap[i], block_color, squareHeight, init_x, init_y);
        else draw(bitmap[i], colors[i-1], squareHeight, init_x, init_y);
    }
    changeDrawSize(2);
}