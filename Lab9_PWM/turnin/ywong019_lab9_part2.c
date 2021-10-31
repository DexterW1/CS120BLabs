/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/147z-BvKwOIn0XHY_TIeRzIZetAcq7F2s/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned short arr[8] = {261.63,293.66,329.63,349.23,392.00,440.00,493.88,523.25};
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
enum States {Start,InitialValues,Up,Down,Toggle,Off}state;
unsigned char toggle = 0;
unsigned char counter =0;
void Tick(){
	tmpA= ~PINA&0x0F;
	switch(state){
		case Start:
			state = InitialValues;
			break;
		case InitialValues:
			if(tmpA== 0x01){
				state = Toggle;
			}
			else if(tmpA==0x02){
				state = Down;
			}
			else if(tmpA==0x04){
				state = Up;
			}
			else{
				state = InitialValues;
			}
			break;
		case Toggle:
			state = InitialValues;
			break;
		case Up:
			state = InitialValues;
			break;
		case Down:
			state = InitialValues;	
			break;
		default:
			break;
	}
	switch(state){
		case Start:set_PWM(0); break;
		case InitialValues:
			if(toggle == 0){
			   set_PWM(arr[counter]);
			}
			else{
				set_PWM(0);
			}
			break;
		case Toggle:
			if(toggle == 0){
				toggle =1;
			}
			else{
				toggle = 0;
			}
			break;
		case Up:
			if(counter < 7){
				counter++;
			}

			break;
		case Down:
			if(counter > 0 ){
				counter--;
			}
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
