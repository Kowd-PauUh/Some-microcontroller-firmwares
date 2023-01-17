/*
 * Task 6
 *
 * Created: 09.01.2023 17:32:08
 * Author : Nemo
 */ 

// MODE1: EVERY KEY
// MODE2: KEY WITH HIGHEST POS
// MODE3: SINGLE KEY. OTHERWISE LEDs OFF

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

// PINB bits:    { 0  1  2  3   4   5   6   7  }
uint8_t LEDs[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
uint8_t keyboard;
uint8_t portc;
uint8_t mode = 1; // MODE SELECTION (1/2/3)

void preset(){
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRB = 0x00;
}

/* Arrange the N elements of ARRAY in random order.
   Only effective if N is much smaller than RAND_MAX;
   if this may not be the case, use a better random
   number generator. */
/*
void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}*/

int log2(float x) {
	return log(x) / log(2);
}

void delay(int ms){
	while(ms){
		_delay_ms(1);
		ms --;
	}
}

int get_input(){
	int port_value = PINB;
	if (port_value != 0xFF){
		delay(40);  // comment for simulation
		if (port_value == PINB){
			delay(40);  // comment for simulation
			return(port_value);
		}
		else {
			return(0xFF);
		}
	}
	else {
		return(0xFF);
	}
}

int main(void)
{
	preset();
	uint8_t mode3_counter = 0;
	//shuffle(arr, 10);  // comment this line to keep the order of the elements in the array
	
    while (1) 
    {
		keyboard = ~get_input();
		
		for (int i = 1; i < 255; i = i << 1) {							// check every key in keyboard
			if (keyboard & i) {											// key number = log2(key value)
				if		(mode == 1) {portc = portc + LEDs[log2(i)];}
				else if (mode == 2) {portc = LEDs[log2(i)];}
				else if (mode == 3) {mode3_counter++; portc = LEDs[log2(i)];}
			}
		}
		
		if (mode == 3 & mode3_counter != 1) {PORTC = 0xFF;}
		else								{PORTC = ~portc;}
		portc = 0; mode3_counter = 0;
		
    }
}
