/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #11  Exercise #5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	
 *	Demo Link: https://drive.google.com/file/d/11SSq0oH6z-qkxrV1M1hMbGm6VGvnxl2k/view?usp=sharing
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
unsigned char Reset_Signal = 0;
unsigned char Start_Signal = 0;
unsigned char Pause_Signal = 0;
unsigned char Lost_Signal = 0;
unsigned char cursor = 1;
enum ButtonStates {ButtonStart,Init,Up,Down,Reset,Press_Once};
int ButtonTick(int state){
	unsigned char tmpA = ~PINA&0x07;
	switch(state){
		case ButtonStart:
			state = Init;
			break;
		case Init:
			if(tmpA==0x01){
				state = Up;
			}
			else if(tmpA==0x02){
				state = Down;
			}
			else if(tmpA==0x04){
				state = Reset;
			}
			else{
				state = Init;
			}
			break;
		case Up:
			state = Press_Once;
			break;
		case Down:
			state = Press_Once;
			break;
		case Reset:
			state = Press_Once;
			break;
		case Press_Once:
			if(tmpA==0x00){
				state = Init;
			}
			else{
				state = Press_Once;
			}
		default:
			break;
	}
	switch(state){
		case ButtonStart:
			Reset_Signal = 0;
			Start_Signal = 0;
			Pause_Signal = 0;
			Lost_Signal = 0;
			break;
		case Init:
			break;
		case Up:
			cursor=1;
			break;
		case Down:
			cursor=17;
			break;
		case Reset:
			if(Lost_Signal == 0){
				Pause_Signal = Pause_Signal ? 0:1;
			}
			else{		
				Reset_Signal = 1;
				Lost_Signal = 0;
			}
			break;
		case Press_Once:
			break;
		default:
			break;
	}
	return state;
}
enum MessageStates{MessageStart,init,Failed};
unsigned char message[]="         *      *      *    *   ";
unsigned char lcd[]="Unlucky, press Restart to play again!";

unsigned char j =0;
int MessageTick(int state){
	switch(state){
		case MessageStart:
			state = init;
			break;
		case init:
			if(message[cursor+j]=='*'){
				state = Failed;
			}
			else{
				state = init;
			}
			break;
		case Failed:
			if(Reset_Signal==1){
				state = init;
			}
			else{
				state = Failed;
			}
		default:
			break;
	}
	switch(state){
		case MessageStart:
			break;
		case init:
			if(Pause_Signal == 0){
				for(int i =1; i<=32;i++){
					LCD_Cursor(i);
					LCD_WriteData(message[(i+j)%32]);
				}
				LCD_Cursor(cursor);
				if(j>=32){
					j=0;
				}
				else{
					j++;
				}
			}
			break;
		case Failed:
			for(j = 1; j<=sizeof(lcd)-1;j++){
				LCD_Cursor(j);
				LCD_WriteData(lcd[j-1]);
			}
			cursor = 1;
			j=0;
			Reset_Signal = 0;
			Lost_Signal = 1;
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
	DDRA= 0x00; PORTA = 0xFF;
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
	task1.TickFct=&ButtonTick;

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
