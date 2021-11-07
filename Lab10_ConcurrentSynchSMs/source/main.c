/*	Author: lab Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: 
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
enum SpeakerStates{BeginSpeaker,SpeakerOn,SpeakerOff,Up,Down,PressOnce}Speakerstate;
unsigned char threeLEDs = 0x00;
unsigned char arr[] ={4,6,8,10,12,14};
unsigned char blinkingLED = 0x00;
unsigned char speaker = 0x00;
unsigned char counter = 0;
unsigned char counter2 = 0;
unsigned char tmpA= 0x00;
void SpeakerTick(){
	tmpA = ~PINA & 0x07;
	switch(Speakerstate){
		case BeginSpeaker:
			Speakerstate = SpeakerOff;
			break;
		case SpeakerOff:
			if(tmpA == 0x04){			
				Speakerstate = SpeakerOn;
			}
			else if (tmpA == 0x01){
				Speakerstate = Up;
			}
			else if (tmpA == 0x02){
				Speakerstate = Down;
			}
			else{
				Speakerstate = SpeakerOff;
			}
			break;
		case SpeakerOn:
			if(tmpA == 0x00){
				Speakerstate = SpeakerOff;
			}
			else{
				Speakerstate = SpeakerOn;
			}
			break;
		case Up:
			Speakerstate = PressOnce;
			break;
		case Down:
			Speakerstate = PressOnce;
			break;

		case PressOnce:
			if(tmpA == 0x00){
				Speakerstate = SpeakerOff;
			}
			else{
				Speakerstate = PressOnce;
			}
		default:
			break;
	}
	switch(Speakerstate){
		case BeginSpeaker:
			break;
		case SpeakerOff:
			speaker = 0x00;
			break;
		case SpeakerOn:
			if(counter <=2){
				speaker = 0x01;
			}
			else if(counter <=(arr[counter2])){
				speaker = 0x00;
			}
			else{
				counter=0;;
			}
			counter++;
			break;
		case Up:
			if(counter2<5){
				counter2++;
			}
			break;
		case Down:
			if(counter2>0){
				counter2--;
			}
			break;
		case PressOnce:
			break;
		default:
			break;
	}

}
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
			PORTB = ((speaker<<4)|(blinkingLED<<3)|threeLEDs);
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
	TimerSet(1);
	TimerOn();
	unsigned short ThreeTime = 0;
	unsigned short BlinkTime= 0;
	unsigned short Period = 1;
	ThreeLEDstate = BeginThree;
	BlinkingLEDstate = BeginBlink;
	CombineLEDstate = BeginCombine;
	Speakerstate = BeginSpeaker;
    while (1) {
	if(ThreeTime >=300){    
		ThreeLEDTick();
		ThreeTime = 0 ;
	}
	if(BlinkTime >=1000){
		BlinkingLEDTick();
		BlinkTime = 0;
	}
	SpeakerTick();
	CombineLEDTick();
	while(!TimerFlag){}
	TimerFlag = 0;
	ThreeTime +=Period;
	BlinkTime +=Period;

    }
    return 1;
}
