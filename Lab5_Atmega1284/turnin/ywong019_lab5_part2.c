/* Author: Yiu Ming Wong 
* Lab Section: 021
 * Assignment: Lab #5  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1LAM2UjQ6TU1PqQsLeLs27X9jUhT_klpq/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

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
				state = Minus;
			}
			else if(tmpA==0x03){
				PORTC=0x00;
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
			state = Minus;
			break;
		case Plus:
			if(tmpA==0x01){
				state = Plus;
			}
			else if(tmpA==0x03) {
				PORTC = 0x00;
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
			PORTC=0x07;
                case InitialValues:
                        break;
                case Reset:
			PORTC= 0x00;
                        break;
		case Plus:
			break;
		case Minus:
			break;
                case Minus_led:
			if(PORTC > 0x00){
                        	PORTC=PORTC-1;
				break;
			}
                        break;
		case Plus_led:
			if(PORTC < 0x09){
				PORTC=PORTC+1;
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
        DDRC= 0xFF; PORTC= 0x00;
    /* Insert your solution below */
    while (1) {
        Tick();
    }
    return 1;
}

