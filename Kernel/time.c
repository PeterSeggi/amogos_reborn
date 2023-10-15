#include <time.h>
#include <ourlib.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	my_ints();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void sleep(int sec){
	unsigned long t0 = ticks_elapsed();
	while( (ticks_elapsed()-t0)/18 < sec );
}

void my_ints(){
	if(ticks%18 == 0){
		printClock();
	}
}