#include <stdint.h>
#include <time.h>
#include <lib.h>

 //Play sound using built in speaker
 static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	_outb(0x43, 0xb6);
 	_outb(0x42, (uint8_t) (Div) );
 	_outb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = _inb(0x61);
  	if (tmp != (tmp | 3)) {
 		_outb(0x61, tmp | 3);
 	}
 }
 
 //make it shutup
 static void nosound() {
 	uint8_t tmp = _inb(0x61) & 0xFC;
 
 	_outb(0x61, tmp);
 }
 
 //Make a beep
 void beep(uint32_t frequency, int duration) {
 	 play_sound(frequency);
 	 sleep(duration, 1);
 	 nosound();
          //set_PIT_2(old_frequency);
 }
