/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA= 0xFF;
	DDRB = 0xFF; PORTB= 0x00;
	DDRB = 0xFF; PORTC= 0x00;
	unsigned char tmpC=0x00;
	unsigned char tmpB=0x00;
	while(1){
		tmpB=0x00;
		tmpC=0x00;
		tmpB=PINA>>4;
		tmpC=PINA<<4;
		PORTB=tmpB;
		PORTC=tmpC;
		
	}

	return 1;
	
}
