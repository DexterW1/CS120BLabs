/* Author: Yiu Ming Wong 
* Lab Section: 021
 * Assignment: Lab #6  Exercise #3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1HsuvTFMc-0W4BhK48olfzCMtRkbtsJqR/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char increment = 0x00;
enum States {Start,Begin,InitialValues,Plus,Plus_led,Minus,Minus_led,Reset}state;
void Tick(){
        unsigned char tmpA=~PINA&0x03;
        switch(state){
                case Start:
                        state=Begin;
                        break;
		case Begin:
			state = InitialValues;
                case InitialValues:
			if(tmpA==0x03){
				state = Reset;
			}
			else if(tmpA==0x02){
				state = Minus_led;
			}
			else if(tmpA==0x01){
				state = Plus_led;
			}
			break;
		case Reset:
			if(tmpA==0x03){
				state = Reset;
			}
			else if(tmpA==0x02){
				state =Minus_led;
			}
			else if(tmpA==0x01){
				state = Plus_led;
			}
			else{

				state = InitialValues;
			}
			break;
		case Minus:
			if(tmpA==0x02){
				increment++; //keep incrementing till it is 10 which will = 1 sec and when its minus the led
				state = Minus;
				if(increment ==10){
					state = Minus_led;
				}
			}
			else if(tmpA==0x03){
				PORTB=0x00;
				state = Reset;
			
			}
			else if(tmpA==0x01){
				state = Plus_led;
			}
			else{
				state = InitialValues;
			}
			break;
		case Minus_led:
			if(increment ==10){
				increment = 0x00;
			}
			state = Minus;
			break;
		case Plus:
			if(tmpA==0x01){
				increment++;// keep incrementing till its 10 and when it reaches 10 = 1 sec and add one led
				state = Plus;
				if(increment==10){
					state = Plus_led;
				}
			}
			else if(tmpA==0x03) {
				PORTB = 0x00;
				state = Reset;	
			}
			else if(tmpA==0x02){
				state = Minus_led;
			}
			else{
				state = InitialValues;
			}
			break;
		case Plus_led:
			if(increment == 10){
				increment = 0x00;
			}
			state = Plus;
			break;
                default:
			state = Start;
                        break;
        }
        switch(state){
                case Start:
                        break;
		case Begin:
			PORTB=0x07;
                case InitialValues:
			increment = 0x00;
                        break;
                case Reset:
			PORTB= 0x00;
                        break;
		case Plus:
			break;
		case Minus:
			break;
                case Minus_led:
			if(PORTB > 0x00){
                        	PORTB=PORTB-1;
				break;
			}
                        break;
		case Plus_led:
			if(PORTB < 0x09){
				PORTB=PORTB+1;
				break;
			}
            		break;
                default:
		
                        break;
        }
}


int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA= 0x00; PORTA=0xFF;
        DDRB= 0xFF; PORTB= 0x00;
	TimerSet(100);
	TimerOn();
    /* Insert your solution below */
    while (1) {
        Tick();
	while(!TimerFlag);
	TimerFlag=0;
    }
    return 1;
}

