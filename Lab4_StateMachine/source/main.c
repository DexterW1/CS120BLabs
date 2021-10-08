/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start,On_Press,On_Release,Off_Press,Off_Release}state;
void Tick(){
	unsigned char tmpA=PINA&0x01;
	switch(state){
		case Start:
			state=On_Press;
			break;
		case On_Press:
			if(tmpA==0x01){
				state =On_Press;
			}else{
				state = On_Release;
			}
			break;
		case On_Release:
			if(tmpA==0x01){
				state = Off_Press;
			}else{
				state = On_Release;
			}
			break;
		case Off_Press:
			if(tmpA==0x01){
				state = Off_Press;
			}
			else{
				state = Off_Release;
			}
			break;
		case Off_Release:
			if(tmpA==0x01){
				state = On_Press;
			}
			else{
				state = Off_Release;
			}
			break;
				
		default:
			break;
	}
	switch(state){
		case Start:
			PORTB=0x01;
			break;
		case On_Press:
			PORTB=0x01;
			break;
		case On_Release:
			break;
		case Off_Press:
			PORTB=0x02;
			break;
		case Off_Release:
			break;
		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA= 0x00; PORTA=0xFF;
	DDRB= 0xFF; PORTB= 0x00;
    /* Insert your solution below */
    while (1) {
	Tick();
    }
    return 1;
}
