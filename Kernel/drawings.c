#include <videoDriver.h>

#define SKYBLUE 0x00010053

void loadKernelScreen(){
    draw_rectangle(getScreenWidth(),getScreenHeight(),SKYBLUE,0,0);
    print("hola ");
    changeFontSize(5);
    print("abcdefghijklmnopqrstuvwxyz");
    while(1);
    /*
    int middleStartX= (getScreenWidth)/2 - ((charWidth*SCALE_MAIN)/2);
	int middleStartY= (getScreenHeight)/2 - ((charHeight*SCALE_MAIN)/2);
    
    uint8_t * title = "AMONGOS";
    uint8_t * loading = "loading...";
    
    int titleStartX = middleStartX - ((charWidth*SCALE_MAIN)*(strlen(title)-1)/2);
	int titleStartY = middleStartY - ((charHeight*SCALE_MAIN)/2);
    
    print(title);
    print(loading);

    for(int i=0; i<5; i++){

    }
    */
}
