/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	
 *	Demo Link:
 */
#include <avr/io.h>
#include "../header/timer.h"
#include "../header/keypad.h"
#include "io.h"
#include "../header/scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char x = 0x00;
unsigned char key = 0x00;
enum KeypadStates {KeyPadStart,Init};
int KeypadTick(int state){
	x = GetKeypadKey();
	switch(state){
		case KeyPadStart: state = Init;
			    break;
		case Init: state = Init;
			   break;
		default:
			   break;
	}
	switch(state){
		case KeyPadStart: break;
		case Init:
			    switch(x){
				case '\0': 	key = 0x1F; break;
				case '1':	key = 0x01; break;
				case '2':	key = 0x02; break;
				case '3':	key = 0x03; break;
				case '4':	key = 0x04; break;
				case '5':	key = 0x05; break;
				case '6':	key = 0x06; break;
				case '7':	key = 0x07; break;
				case '8':	key = 0x08; break;
				case '9':	key = 0x09; break;
				case 'A':	key = 0x0A; break;
				case 'B':	key = 0x0B; break;
				case 'C':	key = 0x0C; break;
				case 'D':	key = 0x0D; break;
				case '*':	key = 0x0E; break;
				case '0':	key = 0x00; break;
				case '#':	key = 0x0F; break;
				default:	key = 0x1B; break;
			    }
			    PORTB=key;
			    break;
		default:
			    break;
	}
	return state;
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0;  PORTC = 0x0F;

	static task task1;
	task *tasks[]={&task1};
	const unsigned short numTasks=sizeof(tasks)/sizeof(task*);
	task1.state = KeyPadStart;
	task1.period = 50;
	task1.elapsedTime=task1.period;
	task1.TickFct=&KeypadTick;
	TimerSet(50);
	TimerOn();
	unsigned short i =0;
    /* Insert your solution below */
    while (1) {
	for(i=0;i<numTasks;i++){
		if(tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state=tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime=0;
		}
		tasks[i]->elapsedTime +=50;
	}
	while(!TimerFlag);
	TimerFlag=0;
    }
    return 1;
}
