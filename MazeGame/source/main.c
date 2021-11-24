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
#include <util/delay.h>
#include "timer.h"
#include "ledmatrix.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned short playerpos;
unsigned short prevplayerpos;
const int lednum[8][8]={
	{0,1,2,3,4,5,6,7},
	{8,9,10,11,12,13,14,15},
	{16,17,18,19,20,21,22,23},
	{24,25,26,27,28,29,30,31},
	{32,33,34,35,36,37,38,39},
	{40,41,42,43,44,45,46,47},
	{48,49,50,51,52,53,54,55},
	{56,57,58,59,60,61,62,63}
	
};
const char level1[8][8]={
	{'#','*','#','@','@','@','#','@'},
	{'@','@','#','@','#','@','#','@'},
	{'@','#','#','@','#','@','@','#'},
	{'@','@','@','@','#','#','@','@'},
	{'#','@','#','#','#','@','@','#'},
	{'@','@','#','@','#','@','#','#'},
	{'@','#','#','@','#','@','@','@'},
	{'@','@','@','@','#','@','#','!'}
};
void printlevel(){
	for(int i=0; i<8;i++){
		for(int j=0; j<8;j++){
			if(level1[i][j]=='#'){
				ld_setled(0,lednum[i][j],1);
			}
			else if(level1[i][j]=='*'){
				playerpos=lednum[i][j];
				ld_setled(0,lednum[i][j],1);
			}
			else if(level1[i][j]=='@'){
				ld_setled(0,lednum[i][j],0);
			}
			else if(level1[i][j]=='!'){
				ld_setled(0,lednum[i][j],1);
			}
		}
	}
}
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
enum MovePlayerStates{BeginMove,MoveInit,Print}Movestate;
void MoveTick(){
	switch(Movestate){
		case BeginMove:
			Movestate=MoveInit;
			break;
		case MoveInit:
			Movestate=Print;
			break;
		case Print:
			Movestate=MoveInit;
			break;
		default:
			break;
	}
	switch(Movestate){
		case BeginMove:
			
			break;
		case MoveInit:
			break;
		case Print:
			ld_setled(0,prevplayerpos,0);
			ld_setled(0,playerpos,1);
			break;
		default:
			break;
	}
}
uint16_t x,y;
enum DirectionStates{BeginDirections,Init,Up,Down,Left,Right,NoInput,Released}Directionstate;
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
			Directionstate = Released;
			break;
		case Down:
			Directionstate = Released;
			break;
		case Left:
			Directionstate = Released;
			break;
		case Right:
			Directionstate = Released;
			break;
		case NoInput:
			Directionstate = Init;
			break;
		case Released:
			if(y>1000 || y<20 || x>1000 || x<20){
				Directionstate = Released;
			}
			else{
				Directionstate = Init;
			}
		default:
			break;
	}
	switch(Directionstate){
		case BeginDirections:
			break;
		case Init:
			break;
		case Up:
			if(playerpos>7){
				prevplayerpos=playerpos;
				playerpos-=8;
			}
			break;
		case Down:
			if(playerpos<56){
				prevplayerpos=playerpos;
				playerpos+=8;
			}
			break;
		case Left:
			if(playerpos!=0 || playerpos!=8 || playerpos!=16||playerpos!=24||playerpos!=32||playerpos!=40||playerpos!=48||playerpos!=56){
				prevplayerpos=playerpos;
				playerpos-=1;
			}
			break;
		case Right:
			if(playerpos!=7||playerpos!=15||playerpos!=23||playerpos!=31||playerpos!=39||playerpos!=47||playerpos!=55||playerpos!=63){
				prevplayerpos=playerpos;
				playerpos+=1;
			}
			break;
		case NoInput:
			break;
		case Released:
			break;
		default:
			break;
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA= 0x00; PORTA = 0xFF;
	DDRB= 0x00; PORTB = 0xFF;
	DDRC= 0xFF; PORTC = 0x00;
	DDRD= 0xFF; PORTD = 0x00;
	char buffer[32];
	LCD_init();
	InitADC();
	ld_init();
	TimerSet(150);
	TimerOn();
	printlevel();
    /* Insert your solution below */
    while (1){
	    x=readadc(0);
	    y=readadc(1);
	    sprintf(buffer,"X=%d            Y=%d",x,y);
	    LCD_DisplayString(1,buffer);
	    DirectionTick();
	    MoveTick();
	    while(!TimerFlag);
	    TimerFlag=0;
    }
    return 1;
}
