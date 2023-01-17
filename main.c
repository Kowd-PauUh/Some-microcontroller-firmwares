/*
 * Task 7
 *
 * Created: 10/17/2022 8:42:31 PM
 *  Author: Nemo
 */ 

/* 
RELEASE ALL BUTTONS BEFORE CONTINUE INTERACTING WITH THE KEYBOARD!

F_CPU/1024 = 1s

     LEDs states as a function of t
^
|          ______                        ____
| LED1:___|      |______________________|
|                         ______
| LED2:__________________|      |____________
+--------------------------------------------> t

*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>

int period = 1;				// DONT CHANGE IT
int duty_cycle = 10;		// [%]
int signal_period = 20;		// [ms]
uint8_t keyboard;


void preset(){
	DDRC = 0xFF;
	PORTC = 0xFF;
	
	TCNT1 = 58336; // = 2^16 - F_CPU/1024
	TCCR1B = (1<<CS10) | (1<<CS12);
	TIMSK = (1<<TOIE1);
	sei();
}

void delay(int ms){
	while(ms){
		_delay_ms(1);
		ms --;
	}
}

void pwm(){
	if (period == 1) {PORTC = 0b01111111;}  // 1st led
	if (period == 2) {PORTC = 0b11111111;}
	
	if (period == 3) {PORTC = 0b10111111;}  // 2nd led
	if (period == 4) {PORTC = 0b11111111;}
}

ISR(TIMER1_OVF_vect){
	if (period != 4) {period++;}
	else {period = 1;}
		
	if (period == 4) {
		TCNT1 = 65536 - round(signal_period / 2 - duty_cycle * signal_period / 2 / 100) *7372800/1024/1000; // = 2^16 - F_CPU/1024
	}
	if (period == 1) {
		TCNT1 = 65536 -round(duty_cycle * signal_period / 2 / 100) *7372800/1024/1000; // = 2^16 - F_CPU/1024
	}
	if (period == 2) {
		TCNT1 = 65536 - round(signal_period / 2 - duty_cycle * signal_period / 2 / 100) *7372800/1024/1000; // = 2^16 - F_CPU/1024
	}
	if (period == 3) {
		TCNT1 = 65536 - round(duty_cycle * signal_period / 2 / 100) *7372800/1024/1000; // = 2^16 - F_CPU/1024
	}
	
	pwm();
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
	int lps = 0; // last pressed status (release all buttons before continue interacting with the keyboard)

    while(1){
		keyboard = ~get_input();
		
		if (keyboard == 0) {lps = 0;}
		
		// Press KEY1 to change the pulse width
		if ((keyboard == 1)&(lps==0)) {
			if (duty_cycle != 90) {duty_cycle = duty_cycle + 10;}
			else {duty_cycle = 10;}
			lps = 1;
		}
	}
}
