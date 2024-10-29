 #include <soundDriver.h>
 #include <lib.h>
 #include <time.h>
 
 /* *** Sound driver ***
  * source: https://wiki.osdev.org/PC_Speaker
  */     
 //Play sound using built-in speaker
void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }
 
 //make it shut up
void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
     
 	outb(0x61, tmp);
 }
 
 //Make a beep
 /*void beep() {
 	 play_sound(1000);
 	 //wait_ticks(10);
	 wait_ticks(18);
 	 nosound();
          //set_PIT_2(old_frequency);
 }*/


 void beep(uint32_t nFrequence, uint32_t time) {
 	 play_sound(nFrequence);
 	 wait_ticks(time);
 	 nosound();
 }
