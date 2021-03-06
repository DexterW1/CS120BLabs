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
	unsigned char my_charB=0x00;
	unsigned char my_charD=0x00;
    /* Insert your solution below */
	ADC_init();
    while (1) {
	unsigned short  my_short = ADC;
	my_charB=(char)my_short;
	my_charD=(char)(my_short>>8);
	PORTB=my_charB;
	PORTD=my_charD;
    }
    return 1;
}
//MAX around 1001111111 = 639 ish;
//MIN around 0000110000 = 48 ish
