#include <time.h>
#include <naiveConsole.h>
#include <miLib.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	/*if(ticks%18 == 0 ){  //si pasa un segundo actualiz el reloj, para testear
		printRTC();
	}
	*/
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

//Ej5 TimerTick

void ej5(int sec){
	unsigned long t0 = ticks_elapsed();
	while( (ticks_elapsed()-t0)/18 < sec );
}


