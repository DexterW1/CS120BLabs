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

unsigned char pressed_once = 0x00;
unsigned char checker = 0x00;
enum States {Start,Begin,Blink,Hold,Reset}state;
void Tick(){
	unsigned char tmpA = ~PINA&0x01;
	switch(state){
		case Start:
			state = Begin;
			break;
		case Begin:
			state = Blink;
			break;
		case Blink:
			if(tmpA == 0x01){
				state = Hold;
			}
			else{
				state = Blink;
			}
			break;
		case Hold:
			if(tmpA==0x01){
				pressed_once = 0x01;
			}
			 if(pressed_once==0x01){
				state = Reset;
			}
			pressed_once = 0x00;
			state = Hold;
		case Reset:
			if(tmpA==0x01){
				pressed_once = 0x01;
			}
			 if(tmpA==0x00){
				state = Reset;
			}
			else if(pressed_once==0x01){
				state = Blink;
			}
			pressed_once = 0x00;
			break;
		default:
			state = Start;
			break;
	}
	switch(state){
		case Start:
			break;
		case Begin:
			PORTB= 0x01;
			break;
		case Blink:
			if(checker == 0x01){
				PORTB = PORTB>>1;
				if(PORTB==0x01){
					checker = 0x00;
				}
			}
			else{
				PORTB = PORTB<<1;
				if(PORTB==0x04){
					checker=0x01;
				}	
			}
			break;
		case Hold:
			break;
		case Reset:
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(300);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
