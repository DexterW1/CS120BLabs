/* Author: Yiu Ming Wong  
* Lab Section: 021
 * Assignment: Lab #6  Exercise #1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: <URL>
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start,Begin,Blink}state;
void Tick(){
	switch(state){
		case Start:
			state = Begin;
			break;
		case Begin:
			state = Blink;
			break;
		case Blink:
			state = Blink;
			break;
		default:
			break;
	}
	switch(state){
		case Start:
			break;
		case Begin:
			PORTB= 0x01;
			break;
		case Blink:
			if(PORTB==0x04){
				PORTB=0x01;
			}
			else{
				PORTB=PORTB<<1; //this will shift from 0001(0x01) to 0010(0x02) to 0100(0x04) if(0100){0x04} then set PORTB back to =0001 or (0x01)
			}
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
