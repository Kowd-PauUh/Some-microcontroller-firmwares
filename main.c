/*
 * Zad4.c
 *
 * Created: 21.11.2022 15:54:34
 *  Author: Nemo
 */ 

// F_CPU/1024 = 1s

#include <avr/io.h>
#include <avr/interrupt.h>
int seconds = 13;

void preset(){
	DDRC = 0xFF;
	PORTC = 0xFF;
	
	TCNT1 = 58336; // = 2^16 - F_CPU/1024
	TCCR1B = (1<<CS10) | (1<<CS12);
	
	// ������������ ��� ������� ���� ��� ����� � ������������, ������������, ���� ������� ��������� �����
	TIMSK = (1<<TOIE1); // zgoda na przerwanie od przekrecenia sie
	sei();
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

// � ������������
ISR(TIMER1_OVF_vect){
	if (seconds != 99) {seconds ++;}
	else {seconds = 0;}
	PORTC = ~bin_bcd(seconds);
	TCNT1 = 58336; // = 2^16 - F_CPU/1024
}

int main(void)
{
	
	preset();
	
    while(1)
    {
		PORTC = ~bin_bcd(seconds);
		
		// ��� ������ � ������� - ������������ ��� ������� � ������� preset()
		
		/*
		// � ��������� ����� TOV1	
		if (TIFR & 0b100) {
			TIFR = 0xFF;
			TCNT1 = 58336;
			if (seconds != 99) {seconds ++;}
			else {seconds = 0;}
		}*/
		
		/*
		// � ��������� ����� OCF1A	
		if (TIFR & 0b10000) {
			TIFR = 0xFF;
			TCNT1 = 58336;
			if (seconds != 99) {seconds ++;}
			else {seconds = 0;}		
		}*/

    }
}