/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #8  Exercise #4
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
	unsigned short MAX=600;
	unsigned short threshold= MAX/8;
    /* Insert your solution below */
	ADC_init();
    while (1) {
	unsigned short  my_short = ADC;

	if(threshold*1 >my_short){
		PORTB=0x01;
	}
	else if(threshold*2 > my_short){
		PORTB = 0x03;
	}
	else if(threshold*3 > my_short){
		PORTB= 0x07;
	}
	else if(threshold*4 > my_short){
		PORTB= 0x0F;
	}
	else if(threshold*5 > my_short){
		PORTB= 0x1F;
        }
	else if(threshold*6 > my_short){
		PORTB= 0X3F;
        }
	else if(threshold*7 > my_short){
		PORTB= 0x7F;
        }
	else if(threshold*8> my_short){
		PORTB = 0xFF;
        }
    }
    return 1;
}
//MAX around 1001111111 = 639 ish;
//MIN around 0000110000 = 48 ish
