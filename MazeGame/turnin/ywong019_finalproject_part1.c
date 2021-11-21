/*	Author: Yiu Ming Wong
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Final Project (part 1 of 3)
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/1KYBTZ4fXATsSRRY2VNpa4MIlHAeH4AkL/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
void InitADC(void)
{
    ADMUX|=(1<<REFS0);
    ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
}
uint16_t readadc(uint8_t ch)
{
    ch&=0b00000111;       
    ADMUX = (ADMUX & 0xf8)|ch;  
    ADCSRA|=(1<<ADSC);       
    while((ADCSRA)&(1<<ADSC));  
    return(ADC);       
}
uint16_t x,y;
enum DirectionStates{BeginDirections,Init,Up,Down,Left,Right,NoInput}Directionstate;
void DirectionTick(){
	x=readadc(0);
	y=readadc(1);
	switch(Directionstate){
		case BeginDirections:
			Directionstate = Init;
			break;
		case Init:
			if(y>1000){
				Directionstate = Up;
			}
			else if(y<20){
				Directionstate = Down;
			}
			else if(x>1000){
				Directionstate = Left;
			}
			else if(x<20){
				Directionstate = Right;
			}
			else if(x==542 && y==155){
				Directionstate = NoInput;
			}
			else{
				Directionstate = Init;
			}
			break;
		case Up:
			if(y>1000){
				Directionstate = Up;
			}
			else{
				Directionstate = Init;
			}
			break;
		case Down:
			if(y<20){
				Directionstate = Down;
			}
			else{
				Directionstate = Init;
			}
			break;
		case Left:
			if(x>1000){
				Directionstate = Left;
			}
			else{
				Directionstate = Init;
			}
			break;
		case Right:
			if(x<20){
				Directionstate = Right;
			}
			else{
				Directionstate = Init;
			}
			break;
		case NoInput:
			Directionstate = Init;
			break;
		default:
			break;
	}
	switch(Directionstate){
		case BeginDirections:
			break;
		case Init:
			PORTB = 0x00;
			break;
		case Up:
			PORTB = 0x02;
			break;
		case Down:
			PORTB = 0x04;
			break;
		case Left:
			PORTB = 0x01;
			break;
		case Right:
			PORTB= 0x08;
			break;
		case NoInput:
			PORTB = 0x00;
			break;
		default:
			break;
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA= 0x00; PORTA = 0xFF;
	DDRB= 0xFF; PORTB = 0x00;
	DDRC= 0xFF; PORTC = 0x00;
	DDRD= 0xFF; PORTD = 0x00;
	char buffer[32];
	LCD_init();
	InitADC();
	TimerSet(300);
	TimerOn();

    /* Insert your solution below */
    while (1){
	    x=readadc(0);
	    y=readadc(1);
	    sprintf(buffer,"X=%d            Y=%d",x,y);
	    LCD_DisplayString(1,buffer);
	    DirectionTick();
	    while(!TimerFlag);
	    TimerFlag=0;
    }
    return 1;
}
