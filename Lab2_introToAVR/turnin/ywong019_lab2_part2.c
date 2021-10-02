/*	Author: Ywong019
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab 2  Exercise 2
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
	DDRB = 0xFF; PORTB = 0x00; //DDRB setting to 1 sets as output pins (PORT = write DDRB as all 0)
	
	unsigned char tmpA=0x00;
	unsigned int cntavail=0x00;

	while(1){
		tmpA=PINA;
		
		for(int i=0; i<4;i++){
			if((tmpA&0x01)==0){
				cntavail +=1;
			}
			tmpA= tmpA>>1;
		}
		PORTC=cntavail;
		cntavail=0x00;		

	}
	

}
