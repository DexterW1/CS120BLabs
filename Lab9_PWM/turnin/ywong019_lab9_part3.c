/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/1lS7ujC6pSl-NCCCNRx2kRYbKSEcotIbx/view?usp=sharing
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
enum States {Start,InitialValues,Play,Toggle}state;
unsigned char toggle = 0;
unsigned char counter =0;
unsigned char counter2 =0;
unsigned char i = 0;
unsigned short C4=261.63; unsigned short D=293.66; unsigned short E = 329.63; unsigned short F= 349.23; unsigned short G = 392.00; unsigned short A= 440.00;
unsigned short B = 493.88; unsigned short C5 = 523.25;
unsigned short melody[28] = {261.63,293.66,329.63,261.63,329.63,261.63,329.63,293.66,329.63,349.23,349.23,329.63,293.66,349.23,329.63,349.23,392.00,329.63,392.00,329.63,392.00,349.23,392.00,440.00,440.00,392.00,349.23,440.00};
unsigned short time[28]= {10,5,5,5,3,3,5,10,5,3,3,3,3,5,10,5,3,3,3,3,3,10,5,3,3,3,3,3};
unsigned char held =0;
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
			else{
				state = InitialValues;
			}
			break;
		case Toggle:
			if(tmpA==0x01){
				state = Toggle;
			}
			else if(tmpA==0x01 && held==0x01){
				state = Toggle;
			}
			else if(tmpA==0x00 && held == 0x01){
				state = InitialValues;
				held = 0;
			}
			else{
				state = Play;
			}
			break;
		case Play:
			if(counter2<=155){
				state = Play;
				counter2++;

			}
			else{
				if(tmpA==0x01){
					held =1;
				}
				state = InitialValues;
			}
		default:
			break;
	}
	switch(state){
		case Start: break;
		case InitialValues:
			set_PWM(0);
			counter = 0;
			counter2 = 0;
			break;
		case Toggle:
			
			break;
		case Play:
			if(i<time[counter]){
				i++;
				set_PWM(melody[counter]);
			}
			else{
				i=0;
				counter++;
				set_PWM(0);
			}
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
