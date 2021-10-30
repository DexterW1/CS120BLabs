/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	
 *	Demo Link: 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
void ADC_init(){
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADATE);
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned short MAX=600;
    /* Insert your solution below */
	ADC_init();
    while (1) {
	unsigned short  my_short = ADC;

	if(my_short >= MAX/2){
		PORTB = 0x01;
	}
	else{
		PORTB = 0x00;
	}
    }
    return 1;
}
//MAX around 1001111111 = 639 ish;
//MIN around 0000110000 = 48 ish
