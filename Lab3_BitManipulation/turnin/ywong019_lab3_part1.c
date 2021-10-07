/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #1
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
	DDRB = 0x00; PORTB= 0xFF;
	DDRC = 0xFF; PORTC= 0x00;
	unsigned char count =0;
	unsigned char tmpA=0x00;
	unsigned char tmpB=0x00;
	unsigned short arr[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    /* Insert your solution below */
    while (1) {
	    tmpA=PINA;
	    tmpB=PINB;
	    count=0x00;
	for(int i=0; i<8;i++){
		if((tmpA & arr[i])==arr[i]){
			count++;
		}
	}
	for(int i=0; i<8;i++){
		if((tmpB & arr[i])==arr[i]){
			count++;
		}
	}
	PORTC=count;
    }
    return 1;
}
