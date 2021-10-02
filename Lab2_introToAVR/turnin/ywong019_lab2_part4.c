/*	Author: Ywong019
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab 2  Exercise 4
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
	DDRA = 0x00; PORTA = 0xFF; //DDRA setting to 0 sets as input pins (PORT = writes DDRA as all 1)
	DDRB = 0x00; PORTB = 0xFF; //DDRB setting to 1 sets as output pins (PORT = write DDRB as all 0)
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;	
	unsigned char tmpA=0x00;
	unsigned char tmpB=0x00;
	unsigned char tmpC=0x00;
	unsigned char tmpD0=0x00;
	unsigned char tmpD1=0x00;
	unsigned char total_weight=0x00;
	while(1){
		tmpA=PINA;
		tmpB=PINB;
		tmpC=PINC;
		total_weight=tmpA+tmpB+tmpC;
		if(total_weight > 0x8C){
			tmpD0=0x01;
		}else{
			tmpD0=0x00;
		}
		if(tmpA>tmpC){
			if((tmpA-tmpC) > 0x50){
				tmpD1=0x02;
			}
		}
		else{
			if((tmpC-tmpA) > 0x50){
				tmpD1=0x02;
			}
		}
		PORTD= (total_weight & 0xFC) | tmpD0 | tmpD1;
		tmpD0=0x00;
		tmpD1=0x00;	
	}
	return 1;

}
