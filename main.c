/*
 * Task 8
 *
 * Created: 10/17/2022 8:42:31 PM
 *  Author: Nemo
 */ 

/*

Press KEY1 to increase the counter
Press KEY2 to decrease the counter
RELEASE ALL BUTTONS THEN CONTINUE INTERACTING WITH THE KEYBOARD

Counter value is displayed in BCD format

*/

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

void preset(){
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRB = 0x00;
}

void delay(int ms){
	while(ms){
		_delay_ms(1);
		ms --;
	}
}

int bin_bcd(int binaryInput){

	int bcdResult = 0;
	int shift = 0;

	while (binaryInput > 0) {
		bcdResult |= (binaryInput % 10) << (shift++ << 2);
		binaryInput /= 10;
	}

	return bcdResult;
}

int get_input(){
	int port_value = PINB;
	if (port_value != 0xFF){
		delay(40);
		if (port_value == PINB){
			delay(40);
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
	int input;
	int8_t clicker = 0;
	int lps = 0; // last pressed status
	
    while(1)
    {
		input = get_input();
		if (input == 0xFF) {lps = 0;}
			
		if ((input != 0xFF)&(lps==0)){
			if (input == 0b11111110){
				if (clicker != 99) {clicker ++;}
				else {clicker = 0;}
				lps = input;
			}
			else if (input == 0b11111101){
				if (clicker != 0) {clicker --;}
				else {clicker = 99;}
				lps = input;
			}
		}
		
		PORTC = ~bin_bcd(clicker);
    }
}
