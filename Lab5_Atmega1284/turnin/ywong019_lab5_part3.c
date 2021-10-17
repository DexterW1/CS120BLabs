/* Author: Yiu Ming Wong
* Lab Section:
 * Assignment: Lab #5  Exercise #3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/16m0z3jHhetzhH0bQelWfvSMgKryxVDJk/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned arr[12]={0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x21,0x12,0x0C,0x15,0x2A};
unsigned char counter =0x00;
enum States {Start,Begin,InitialValues,Plus,Plus_led,Reset}state;
void Tick(){
        unsigned char tmpA=~PINA&0x01;
        switch(state){
                case Start:
                        state=Begin;
                        break;
		case Begin:
			state = InitialValues;
                case InitialValues:
			if(counter>12){
				state = Reset;
			}
			else if(tmpA==0x01){
				state = Plus_led;
			}
			break;
		case Reset:
			state = InitialValues;	
			break;
		case Plus:
			if(tmpA==0x01){
				state = Plus;
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
			PORTC=0x00;
                case InitialValues:
                        break;
                case Reset:
			PORTC= 0x00;
			counter=0x00;
                        break;
		case Plus:
			break;
		case Plus_led:
			PORTC=arr[counter];
			counter++;
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

