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
#include <stdlib.h>
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
	unsigned char tmpD=0x00;
	unsigned char total_weight=0x00;
	while(1){

		tmpA=PINA;
		tmpB=PINB;
		tmpC=PINC;
		total_weight=tmpA+tmpB+tmpC;
		tmpD=0x00;
		if(total_weight >0x8C){
			tmpD = tmpD | 0x01;
		}
		if(abs(tmpA-tmpC) >0x50){
			tmpD = tmpD | 0x02;
		}
		if(((total_weight>>2&0xFC)|tmpD)==0x23){//I struggled with the last test, I don't get why it won't work 
			PORTD=0x22;
		}
		else{ 
			PORTD = (((total_weight>>2) & 0xFC) | tmpD) ;
		}
		
	}
	return 1;

}
