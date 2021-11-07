/*	Author: lab Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/1o02Mjt0k3f5BBRFgEqT5F239bH3MAVyc/view?usp=sharing 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum ThreeLEDsSMStates{BeginThree,bitZero,bitOne,bitTwo}ThreeLEDstate;
enum BlinkingLEDSMStates{BeginBlink,blinkOn,blinkOff}BlinkingLEDstate;
enum CombineLEDsSMStates{BeginCombine,Output}CombineLEDstate;
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;

void ThreeLEDTick(){
	switch(ThreeLEDstate){
		case BeginThree:
			ThreeLEDstate = bitZero;
			break;
		case bitZero:
			ThreeLEDstate = bitOne;
			break;
		case bitOne:
			ThreeLEDstate = bitTwo;
			break;
		case bitTwo:
			ThreeLEDstate=bitZero;
			break;
		default:
			break;
	}
	switch(ThreeLEDstate){
		case BeginThree: break;
		case bitZero: threeLEDs = 0x01; break;
		case bitOne: threeLEDs = 0x02; break;
		case bitTwo: threeLEDs = 0x04; break;
		default: break;
	}
}
void BlinkingLEDTick(){
	switch(BlinkingLEDstate){
		case BeginBlink:
			BlinkingLEDstate = blinkOn;
			break;
		case blinkOff:
			BlinkingLEDstate = blinkOn;
			break;
		case blinkOn:
			BlinkingLEDstate = blinkOff;
			break;
		default:
			break;
	}
	switch(BlinkingLEDstate){
		case BeginBlink: break;
		case blinkOff: blinkingLED = 0x00; break;
		case blinkOn: blinkingLED = 0x01; break;
		default: break;
	}
}
void CombineLEDTick(){
	switch(CombineLEDstate){
		case BeginCombine: CombineLEDstate = Output; break;
		case Output: CombineLEDstate = Output; break;
		default: break;
	}

	switch(CombineLEDstate){
		case BeginCombine: break;
		case Output:
			PORTB = ((blinkingLED<<3)|threeLEDs);
			break;
		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	TimerSet(1000);
	TimerOn();
	ThreeLEDstate = BeginThree;
	BlinkingLEDstate = BeginBlink;
	CombineLEDstate = BeginCombine;
    while (1) {
	ThreeLEDTick();
	BlinkingLEDTick();
	CombineLEDTick();
	while(!TimerFlag){}
	TimerFlag = 0;

    }
    return 1;
}
