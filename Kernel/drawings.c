#include <drawings.h>
#include <videoDriver.h>
#include <fonts.h>
#include <time.h>


#define SKYBLUE 0x00010053
#define TITLEGOLD 0x00FAEF02

void loadKernelScreen(){
    draw_rectangle(getScreenWidth(),getScreenHeight(),SKYBLUE,0,0);
    changeDrawSize(10);

    int middleStartX= (getScreenWidth())/2 - ((charWidth*getDrawSize())/2);
	int middleStartY= (getScreenHeight())/2 - ((charHeight*getDrawSize())/2);
    
    char * title = "AMOGOS";

    int titleStartX = middleStartX - ((charWidth*getDrawSize())*(5)/2);//fixed strlen in 6 (5=strlen-1)
	int titleStartY = middleStartY - ((charHeight*getDrawSize())/2);
    
    for(int i=0; title[i]!=0; i++){
        printBitmap(font[title[i]], TITLEGOLD, charHeight, titleStartX + (charWidth*getDrawSize()*(i)), titleStartY);
    }

    int loadStartY = middleStartY + (charHeight*getDrawSize());

    changeDrawSize(3);

    int loadStartX = (getScreenWidth())/2 - ((charWidth*getDrawSize()*(7))*(0.5));//fixed strlen in 7 (6=strlen-1)
	

    char* loading = "loading...";
    for(int i=0; loading[i]!=0; i++){
        printBitmap(font[loading[i]], TITLEGOLD, charHeight, loadStartX + (charWidth*getDrawSize()*(i)), loadStartY);
    }

    changeDrawSize(10);

    for(int i=0; i<4; i++){
        printBitmap(dibujitos[9], SKYBLUE, charHeight, middleStartX, middleStartY);
        printBitmap(dibujitos[0], TITLEGOLD, charHeight, middleStartX, middleStartY);
        sleep(1);
        printBitmap(dibujitos[9], SKYBLUE, charHeight, middleStartX, middleStartY);
        printBitmap(dibujitos[1], TITLEGOLD, charHeight, middleStartX, middleStartY);
        sleep(1);
    }
}
