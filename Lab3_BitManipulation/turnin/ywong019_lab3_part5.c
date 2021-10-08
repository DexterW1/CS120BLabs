/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #5
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
	DDRD = 0x00; PORTD= 0xFF;  //DDR decides if pin is going to be input(0) or output(1) in this case //DDRB (0000 0000) sets all to input
	DDRB = 0xFE; PORTB= 0x00;
	unsigned char tmpD=0x00;
	unsigned char tmpB=0x00;
	unsigned char holdB=0x00;
	unsigned char check_overshift=0;
	unsigned short total_weight =0x00;
	while(1){

		check_overshift =0;
		if(PIND==0x80){
			check_overshift = 1;
		}
		tmpD= PIND<<1;
		tmpB=PINB&0x01;
		holdB=0x00;
		total_weight =tmpD+tmpB;

		if(total_weight >= 0x46|| check_overshift ==1){
			holdB=0x02; //(0000 0010)
		}
		else if(total_weight > 0x05 && total_weight < 0x46){
			holdB=0x04; //(0000 01000)
		}
		
		PORTB=holdB;


		
	}

	return 1;
	
}
