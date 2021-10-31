/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/18EdLCaEU0cv_6bgxRQf2sDfCW8AMhn4h/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char tmpA = 0x00;
void set_PWM(double frequency){
	static double current_frequency;
	if(frequency != current_frequency){
		if(!frequency){TCCR3B &=0x08;}
		else{TCCR3B |=0x03;}
		
		if(frequency <0.954){OCR3A=0xFFFF;}
		
		else if(frequency > 31250){OCR3A=0x0000;}

		else{OCR3A=(short)(8000000/(128*frequency))-1;}

		TCNT3=0;
		current_frequency = frequency;

	}
}
void PWM_on(){
	TCCR3A=(1<<COM3A0);
	TCCR3B=(1<<WGM32) | (1<<CS31) | (1<<CS30);
	set_PWM(0);
}
void PWM_off(){
	TCCR3A=0x00;
	TCCR3B=0x00;
}
enum States {Start,InitialValues,C,D,E}state;
void Tick(){
	tmpA= ~PINA&0x0F;
	switch(state){
		case Start:
			state = InitialValues;
			break;
		case InitialValues:
			if(tmpA== 0x01){
				state = C;
			}
			else if(tmpA==0x02){
				state = D;
			}
			else if(tmpA==0x04){
				state = E;
			}
			else{
				state = InitialValues;
			}
			break;
		case C:
			if(tmpA== 0x01){
				state = C;
			}
			else{
				state = InitialValues;
			}
			break;
		case D:
			if(tmpA==0x02){
				state = D;
			}
			else{
				state = InitialValues;
			}
			break;
		case E:
			if(tmpA==0x04){
				state = E;
			}
			else{
				state = InitialValues;
			}
			break;
		default:
			break;
	}
	switch(state){
		case Start: break;
		case InitialValues:
			set_PWM(0);
			break;
		case C:
			set_PWM(261.63);
			break;
		case D:
			set_PWM(293.66);
			break;
		case E:
			set_PWM(329.63);
			break;
		default:
			break;
	}

}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	PWM_on();
    /* Insert your solution below */
    while (1) {
	    Tick();
	    while(!TimerFlag){};
	    TimerFlag=0;
    }
    return 1;
}
