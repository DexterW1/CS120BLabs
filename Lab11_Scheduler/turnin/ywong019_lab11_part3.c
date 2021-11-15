/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #11  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	
 *	Demo Link: https://drive.google.com/file/d/15GQxcTubEA63n7KmxFyijqMlWmt1U5jZ/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../header/timer.h"
#include "../header/keypad.h"
#include "../header/io.h"
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
enum MessageStates{MessageStart,init,Display};
unsigned char message[]="               CS120B is Legend... wait for it DARY!";
unsigned char lcd[16];
unsigned char cursor = 0x00;
int MessageTick(int state){
	switch(state){
		case MessageStart:
			state = init;
			break;
		case init:
			state = Display;
			break;
		case Display:
			state  = init;
		default:
			break;
	}
	switch(state){
		case MessageStart:
			break;
		case init:
			break;
		case Display:
			LCD_Cursor(1);
			LCD_WriteData(x);
		default:
			break;
	}
	return state;
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0;  PORTC = 0x0F;
	DDRA= 0xFF; PORTA = 0x00;
	DDRD= 0xFF; PORTB= 0x00;
	LCD_init();
	static task task1;
	static task task2;
	task *tasks[]={&task1,&task2};
	const unsigned short numTasks=sizeof(tasks)/sizeof(task*);
	const char start =0;
	task1.state = start;
	task1.period = 10;
	task1.elapsedTime=task1.period;
	task1.TickFct=&KeypadTick;

	task2.state=start;
	task2.period = 20;
	task2.elapsedTime=task2.period;
	task2.TickFct=&MessageTick;
	TimerSet(10);
	TimerOn();
	unsigned short i =0;
    /* Insert your solution below */
    while (1) {
	for(i=0;i<numTasks;i++){
		if(tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state=tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime=0;
		}
		tasks[i]->elapsedTime +=1;
	}
	while(!TimerFlag);
	TimerFlag=0;
    }
    return 1;
}
