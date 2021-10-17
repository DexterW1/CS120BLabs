/*      Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *      Lab Section: 021
 *      Assignment: Lab #4  Exercise #1
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start,InitialValues,Button_Y,Button_HashTag,Locked,Unlocked}state;
void Tick(){
	unsigned char tmpA=PINA&0x87;
        switch(state){
                case Start:
                        state=InitialValues;
                        break;
                case InitialValues:
			if(tmpA==0x04){
				state = Button_HashTag;
			}
			else{
				state = InitialValues;
			}
			break;
		case Button_HashTag:
			if(tmpA==0x02){
				state = Button_Y;
			}
			else if(tmpA==0x00){
				state= Button_HashTag;
			}
			else{
				state = Locked;
			}
			break;
		case Unlocked:
			if(tmpA==0x80){
				state = Locked;
			}
			else{
				state = InitialValues;
			}
			break;
		case Locked:
			if(tmpA==0x80){
				state = Locked;
			}
			else{
				state = InitialValues;
			}
			break;
		case Button_Y:
			if(tmpA==0x02){
				state = Button_Y;
			}
			else if(tmpA==0x00){
				state = Unlocked;
			}
			else if(tmpA==0x00 && PORTB==0x01){
				state = Locked;
			}
			else{
				state = InitialValues;
			}
			break;
                default:
			state = Start;
                        break;
        }
        switch(state){
                case Start:
			PORTB = 0x00;
                        break;
                case InitialValues:
                        break;
                case Button_HashTag:
                        break;
		case Unlocked:
			PORTB=0x01;
			break;
		case Locked:
			PORTB=0x00;
			break;
                case Button_Y:
			PORTB=0x01;
                        break;
                default:
			PORTB = 0x00;
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
