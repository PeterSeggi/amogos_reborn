#include <videoDriver.h>
#include <time.h>
#include <fonts.h>

typedef struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y){
	uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
	uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch); //3 ya que son RGB(3) (24bits)
	framebuffer[offset]=(hexColor) & 0x000000FF;	//agarro la parte baja del hexColor que es el azul
	framebuffer[offset+1]=(hexColor>>8) & 0x0000FF;
	framebuffer[offset+2]=(hexColor>>16) & 0x00FF;
}

void draw_rectangle(uint64_t ancho, uint64_t alto, uint32_t color, uint64_t init_x, uint64_t init_y){
	for(uint64_t i=0; i<ancho; i++){
		for(uint64_t j=0;j<alto;j++){
			putPixel(color,i+init_x,j+init_y);
		}
	}
}

//================================================================================================================================
// Text for videoMode
//================================================================================================================================
//================================================================================================================================
#define SCALE_REAL 1
#define SCALE 10
#define SCALE_MAIN 5

void putChar(uint8_t character, uint32_t colorFont, uint32_t colorBg, uint64_t init_x, uint64_t init_y){
	for(uint64_t i=0; i<(charHeight*SCALE); i++){
		for(uint64_t j=0; j<(charWidth*SCALE); j++){
			if( ((font[character][(i/SCALE)]<<(j/SCALE)) & (1<<(charWidth-1))) ){// 1<<charWidth permite leer de a un bit de izq a der del row de la font
				putPixel(colorFont,init_x+j,init_y+i);
			}
			else{
				putPixel(colorBg,init_x+j,init_y+i);
			}
			
		}
	}
}

void putCharMain(uint8_t character, uint32_t colorFont, uint32_t colorBg, uint64_t init_x, uint64_t init_y){
	for(uint64_t i=0; i<(charHeight*SCALE_MAIN); i++){
		for(uint64_t j=0; j<(charWidth*SCALE_MAIN); j++){
			if( ((font[character][(i/SCALE_MAIN)]<<(j/SCALE_MAIN)) & (1<<(charWidth-1))) ){// 1<<charWidth permite leer de a un bit de izq a der del row de la font
				putPixel(colorFont,init_x+j,init_y+i);
			}
			else{
				putPixel(colorBg,init_x+j,init_y+i);
			}
			
		}
	}
}

void putDibujito(uint8_t character, uint32_t colorFont, uint32_t colorBg, uint64_t init_x, uint64_t init_y){
	for(uint64_t i=0; i<(charHeight*SCALE); i++){
		for(uint64_t j=0; j<(charWidth*SCALE); j++){
			if( ((dibujitos[character][(i/SCALE)]<<(j/SCALE)) & (1<<(charWidth-1))) ){// 1<<charWidth permite leer de a un bit de izq a der del row de la font
				putPixel(colorFont,init_x+j,init_y+i);
			}
			else{
				putPixel(colorBg,init_x+j,init_y+i);
			}
			
		}
	}
}

void putDibujitoMain(uint8_t character, uint32_t colorFont, uint32_t colorBg, uint64_t init_x, uint64_t init_y){
	for(uint64_t i=0; i<(charHeight*SCALE_MAIN); i++){
		for(uint64_t j=0; j<(charWidth*SCALE_MAIN); j++){
			if( ((dibujitos[character][(i/SCALE_MAIN)]<<(j/SCALE_MAIN)) & (1<<(charWidth-1))) ){// 1<<charWidth permite leer de a un bit de izq a der del row de la font
				putPixel(colorFont,init_x+j,init_y+i);
			}
			else{
				putPixel(colorBg,init_x+j,init_y+i);
			}
			
		}
	}
}

void putDibujitoMainOnly(uint8_t character, uint32_t colorFont, uint64_t init_x, uint64_t init_y){
	for(uint64_t i=0; i<(charHeight*SCALE_MAIN); i++){
		for(uint64_t j=0; j<(charWidth*SCALE_MAIN); j++){
			if( ((dibujitos[character][(i/SCALE_MAIN)]<<(j/SCALE_MAIN)) & (1<<(charWidth-1))) ){// 1<<charWidth permite leer de a un bit de izq a der del row de la font
				putPixel(colorFont,init_x+j,init_y+i);
			}			
		}
	}
}

void putDibujitoReal(uint8_t character, uint32_t colorFont, uint32_t colorBg, uint64_t init_x, uint64_t init_y){
	for(uint64_t i=0; i<(charHeight*SCALE_REAL); i++){
		for(uint64_t j=0; j<(charWidth*SCALE_REAL); j++){
			if( ((dibujitos[character][(i/SCALE_REAL)]<<(j/SCALE_REAL)) & (1<<(charWidth-1))) ){// 1<<charWidth permite leer de a un bit de izq a der del row de la font
				putPixel(colorFont,init_x+j,init_y+i);
			}
			else{
				putPixel(colorBg,init_x+j,init_y+i);
			}
			
		}
	}
}

void writeStar(){
	int middleStartX= (VBE_mode_info->width)/2 - ((charWidth*SCALE_MAIN)/2);
	int middleStartY= (VBE_mode_info->height)/2 - ((charHeight*SCALE_MAIN)/2);
	int title_len=6;
	int titleStartX = middleStartX - ((charWidth*SCALE_MAIN)*(title_len-1)/2);
	int titleStartY = middleStartY - ((charHeight*SCALE_MAIN)/2);

	//putDibujito(7,0x00FAEF02,0x00010053,middleStartX,middleStartY+(charHeight*SCALE_MAIN));

	#define YELLOW 0x00FAE425
	#define LIGHTGRAY 0x00B8B8B8
	#define WHITELINE 0x00FBFBFB
	#define WHITE 0x00F6F6F6
	#define BLACK 0x00000000
	#define DARKGRAY 0x00414141
	#define DARKERGRAY 0x00343434
	#define RED 0x00FF290E

	int start_time=ticks_elapsed();
	//putDibujitoMain(0,0x00FAEF02,0x00010053,middleStartX,middleStartY);

	putDibujitoMainOnly(41,LIGHTGRAY,middleStartX-(charWidth*SCALE_MAIN*2),middleStartY);
	putDibujitoMainOnly(40,YELLOW,middleStartX-(charWidth*SCALE_MAIN*2),middleStartY);
	putDibujitoMainOnly(39,DARKGRAY,middleStartX-(charWidth*SCALE_MAIN*2),middleStartY);
	putDibujitoMainOnly(38,BLACK,middleStartX-(charWidth*SCALE_MAIN*2),middleStartY);

	putDibujitoMainOnly(44,YELLOW,middleStartX,middleStartY-(charWidth*SCALE_MAIN));
	putDibujitoMainOnly(43,DARKGRAY,middleStartX,middleStartY-(charWidth*SCALE_MAIN));
	putDibujitoMainOnly(42,BLACK,middleStartX,middleStartY-(charWidth*SCALE_MAIN));

	putDibujitoMainOnly(22,LIGHTGRAY,middleStartX-(charWidth*SCALE_MAIN),middleStartY);
	putDibujitoMainOnly(21,YELLOW,middleStartX-(charWidth*SCALE_MAIN),middleStartY);
	putDibujitoMainOnly(20,DARKGRAY,middleStartX-(charWidth*SCALE_MAIN),middleStartY);
	putDibujitoMainOnly(19,WHITELINE,middleStartX-(charWidth*SCALE_MAIN),middleStartY);
	putDibujitoMainOnly(18,BLACK,middleStartX-(charWidth*SCALE_MAIN),middleStartY);

	putDibujitoMainOnly(37,LIGHTGRAY,middleStartX,middleStartY);
	putDibujitoMainOnly(36,YELLOW,middleStartX,middleStartY);
	putDibujitoMainOnly(35,DARKGRAY,middleStartX,middleStartY);
	putDibujitoMainOnly(34,BLACK,middleStartX,middleStartY);

	putDibujitoMainOnly(12,YELLOW,middleStartX-(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(11,DARKGRAY,middleStartX-(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(10,WHITELINE,middleStartX-(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(9,WHITE,middleStartX-(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(8,BLACK,middleStartX-(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));

	putDibujitoMainOnly(22,LIGHTGRAY,middleStartX-(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(21,YELLOW,middleStartX-(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(20,DARKGRAY,middleStartX-(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(19,WHITELINE,middleStartX-(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(18,BLACK,middleStartX-(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));

	putDibujitoMainOnly(27,YELLOW,middleStartX,middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(26,LIGHTGRAY,middleStartX,middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(25,DARKGRAY,middleStartX,middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(24,WHITELINE,middleStartX,middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(23,BLACK,middleStartX,middleStartY+(charHeight*SCALE_MAIN));

	putDibujitoMainOnly(22,LIGHTGRAY,middleStartX+(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(21,YELLOW,middleStartX+(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(20,DARKGRAY,middleStartX+(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(19,WHITELINE,middleStartX+(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(18,BLACK,middleStartX+(charWidth*SCALE_MAIN),middleStartY+(charHeight*SCALE_MAIN));

	putDibujitoMainOnly(17,YELLOW,middleStartX+(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(16,LIGHTGRAY,middleStartX+(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(15,DARKGRAY,middleStartX+(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(14,RED,middleStartX+(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));
	putDibujitoMainOnly(13,BLACK,middleStartX+(charWidth*SCALE_MAIN*2),middleStartY+(charHeight*SCALE_MAIN));
	
	uint8_t starFlag=1;
	//putChar('A',0x00FAEF02,0x00010053,titleStartX,titleStartY);
	//putChar('M',0x00FAEF02,0x00010053,titleStartX+(charWidth*SCALE_MAIN),titleStartY);
	//putChar('O',0x00FAEF02,0x00010053,titleStartX+(charWidth*SCALE_MAIN*2),titleStartY);
	//putChar('G',0x00FAEF02,0x00010053,titleStartX+(charWidth*SCALE_MAIN*3),titleStartY);
	//putChar('O',0x00FAEF02,0x00010053,titleStartX+(charWidth*SCALE_MAIN*4)+4,titleStartY);
	//putChar('S',0x00FAEF02,0x00010053,titleStartX+(charWidth*SCALE_MAIN*5)+4,titleStartY);
	while(1){
		//if(ticks_elapsed()-start_time>18){
			//if(starFlag) putDibujitoMain(0,0x00FAEF02,0x00010053,middleStartX,middleStartY);
			//else putDibujitoMain(1,0x00FAEF02,0x00010053,middleStartX,middleStartY);
			//starFlag=!starFlag;
			//start_time=ticks_elapsed();
		//}
	}
}

void writeMoon(uint16_t x, uint16_t y){
	putDibujito(6,0x00FAEF02,0x00010053,x,y);
}

void writeSky(){
	int marginX = (VBE_mode_info->width) % (charWidth*SCALE_REAL);
	int marginY = (VBE_mode_info->height) % (charHeight*SCALE_REAL);

	for(int i=0;i<VBE_mode_info->height;i++){
		for(int j=0;j<VBE_mode_info->width;j++){
			putPixel(0x00010053,j,i);
		}
	}
}

//================================================================================================================================

//================================================================================================================================
// Sound testing for videoMode
//================================================================================================================================
//================================================================================================================================

void beep(){
	talk(1000);
	ticks_wait(36);
	shut_up();
}

void talk(uint32_t freq){
	uint32_t div;

	//PIT settings para la freq
	div = (1193180)/freq;	//input signal de 1.193182 MHz
	_PITsetup(div);

	//ahora que empiece a sonar
	_talk();
}

void shut_up(){
	_shutup();
}