/* Author: Yiu Ming Wong  
* Lab Section: 021
 * Assignment: Lab #6  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/12O605J8VPLDOCO7qPBVv25wFGUMTIv7N/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char pressed_once = 0x00;
unsigned char checker = 0x00;
unsigned char score = 0x05;
unsigned char checker2 = 0x00;
unsigned char win = 0x00;
enum States {Start,Begin,Blink,Hold,Reset}state;
void Tick(){
	unsigned char tmpA = ~PINA&0x03;
	switch(state){
		case Start:
			state = Begin;
			break;
		case Begin:
			state = Blink;
			break;
		case Blink:
			if(tmpA == 0x01){
				state = Hold;
			}
			else{
				state = Blink;
			}
			break;
		case Hold:
			if(tmpA==0x01){
				pressed_once = 0x01;
			}
			 if(pressed_once==0x01){
				state = Reset;
			}
			pressed_once = 0x00;
			state = Hold;
		case Reset:
			if(tmpA==0x01){
				pressed_once = 0x01;
			}
			 if(tmpA==0x00){
				state = Reset;
			}
			else if(pressed_once==0x01){
				state = Blink;
			}
			pressed_once = 0x00;
			break;
		default:
			state = Start;
			break;
	}
	switch(state){
		case Start:
			break;
		case Begin:
			PORTB= 0x01;
			break;
		case Blink:
			if(checker == 0x01){
				PORTB = PORTB>>1;
				if(PORTB==0x01){
					checker = 0x00;
				}
			}
			else{
				PORTB = PORTB<<1;
				if(PORTB==0x04){
					checker=0x01;
				}	
			}
			break;
		case Hold:
			if(PORTB == 0x02){
				if(score <=9){
					score++;
				}
				if(score == 9){
					win=0x01;
				}
			}
			else{
				if(score > 0){
					score--;
				}
			}
			break;
		case Reset:
			break;
		default:
			break;
	}
	LCD_Cursor(1);
	if(win !=1){
		LCD_ClearScreen();
		LCD_WriteData(score + '0');
	}else{
		LCD_ClearScreen();
		LCD_DisplayString(1,"You won lets GOOOOOO");
		if(PORTB != 0x02){
			win = 0x00;
			score = 0x05;
		}
	}	
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC =0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(300);
	TimerOn();
	LCD_init();
    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
