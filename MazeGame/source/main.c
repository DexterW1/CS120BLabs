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
#include <stdbool.h>
#include <avr/interrupt.h>
#include "io.h"
#include <util/delay.h>
#include "timer.h"
#include "ledmatrix.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned short playerpos;
bool checkwin = false;
int check_level = 0;
int current_level = 0;
unsigned short win_num;
unsigned short prevplayerpos=9999;
int *collisionarr;
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
const char level[4][8][8]={
        {
                {'#','*','#','@','@','@','#','@'},
                {'@','@','#','@','#','@','#','@'},
                {'@','#','#','@','#','@','@','#'},
                {'@','@','@','@','#','#','@','@'},
                {'#','@','#','#','#','@','@','#'},
                {'@','@','#','@','#','@','#','#'},
                {'@','#','#','@','#','@','@','@'},
                {'@','@','@','@','#','@','#','!'}
        },
        {
                {'*','#','@','@','@','#','#','!'},
                {'@','#','@','#','@','#','#','@'},
                {'@','@','@','#','#','#','#','@'},
                {'#','@','@','#','@','#','#','@'},
                {'@','@','@','#','@','@','@','@'},
                {'@','#','@','#','@','#','@','#'},
                {'@','#','@','@','@','#','@','#'},
                {'@','@','@','@','@','#','@','#'}
        },
        {
                {'#','@','@','@','@','@','@','#'},
		{'@','@','#','#','#','#','@','@'},
		{'@','#','@','@','#','#','@','#'},
		{'@','@','@','*','#','#','@','#'},
		{'#','#','#','#','@','@','@','@'},
		{'@','@','@','#','@','#','#','@'},
		{'@','#','@','#','#','@','@','@'},
		{'!','#','@','@','@','@','#','@'}
        },
	{
		{'@','@','@','@','#','@','#','*'},
		{'#','@','#','@','#','@','#','@'},
		{'@','@','#','@','#','@','@','@'},
		{'@','#','#','!','#','#','#','@'},
		{'@','@','#','#','#','#','@','@'},
		{'@','#','@','@','@','#','@','#'},
		{'@','@','@','#','@','@','@','@'},
		{'#','@','@','#','@','@','#','@'},
	}

};

int * collision(){
	static int arr[63];
	int counter = 0;
	for(int i=0; i<8;i++){
		for(int j=0; j<8;j++){
			if(level[current_level][i][j]=='#'){
				arr[counter]=lednum[i][j];
				counter++;
			}
		}
	}
	return arr;

}
bool collisionExist(int arr[],int target){
	for(int i=0; i<63;i++){
		if(target==arr[i]){
			return true;
		}
	}
	return false;
}
void printlevel(){
	for(int i=0; i<8;i++){
		for(int j=0; j<8;j++){
			if(level[current_level][i][j]=='#'){
				ld_setled(0,lednum[i][j],1);

			}
			else if(level[current_level][i][j]=='*'){
				playerpos=lednum[i][j];
				ld_setled(0,lednum[i][j],1);
			}
			else if(level[current_level][i][j]=='@'){
				ld_setled(0,lednum[i][j],0);
			}
			else if(level[current_level][i][j]=='!'){
				ld_setled(0,lednum[i][j],1);
				win_num=lednum[i][j];
			}
		}
	}
}
void printlevelOff(){
        for(int i=0; i<8;i++){
                for(int j=0; j<8;j++){
                        if(level[current_level][i][j]=='#'){
                                ld_setled(0,lednum[i][j],0);

                        }
                }
        }

}
void LightAll(){
	ld_resetmatrix(1);
	ld_resetmatrix(0);
	for(int i=0; i<8;i++){
		for(int j=0; j<8;j++){
			ld_setled(0,lednum[i][j],1);
		}
	}
}
void BlinkWinOn(){
	ld_setled(0,win_num,0);
}
void BlinkWinOff(){
	ld_setled(0,win_num,1);
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
enum LCDScoreStates{BeginLCD,LCDInit,LCDUpdate,LCDWinner}LCDstate;
enum MovePlayerStates{BeginMove,MoveInit,Print,Winner,NextLevel}Movestate;
void MoveTick(){
	unsigned char tmpA = ~PINA&0x08;
	unsigned int time_count =0;
	switch(Movestate){
		case BeginMove:
			Movestate=MoveInit;
			break;
		case MoveInit:
			if(playerpos == win_num){
				Movestate = NextLevel;
			}
			else if (current_level==4){
				Movestate = Winner;
			}
			else{
				Movestate=Print;
			}
			break;
		case Print:
			Movestate=MoveInit;
			break;
		case NextLevel:
			Movestate = MoveInit;
			break;
		case Winner:
			if(tmpA==0){
				Movestate = Winner;
				LightAll();
			}
			else{
				Movestate = MoveInit;
			}
			break;
		default:
			break;
	}
	switch(Movestate){
		case BeginMove:
			
			break;
		case MoveInit:
			ld_setled(0,playerpos,0);
			break;
		case Print:
			ld_setled(0,prevplayerpos,0);
			ld_setled(0,playerpos,1);
			break;
		case Winner:
			checkwin=true;
			break;
		case NextLevel:
			checkwin  = true;
			current_level++;
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
	collisionarr = collision();
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
			if(playerpos>7 && !collisionExist(collisionarr,playerpos-8)){
				prevplayerpos=playerpos;
				playerpos-=8;
			}
			break;
		case Down:
			if(playerpos<56&& !collisionExist(collisionarr,playerpos+8)){
				prevplayerpos=playerpos;
				playerpos+=8;
			}
			break;
		case Left:
			if(playerpos!=0 && playerpos!=8 && playerpos!=16&&playerpos!=24&&playerpos!=32&&playerpos!=40&&playerpos!=48&&playerpos!=56&&!collisionExist(collisionarr,playerpos-1)){
				prevplayerpos=playerpos;
				playerpos-=1;
			}
			break;
		case Right:
			if(playerpos!=7&&playerpos!=15&&playerpos!=23&&playerpos!=31&&playerpos!=39&&playerpos!=47&&playerpos!=55&&playerpos!=63&&!collisionExist(collisionarr,playerpos+1)){
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
	const unsigned long timeperiod = 150;
	unsigned long blinkwin_on =0;
	unsigned long blinkwin_off =0;
	unsigned long printlevel_on=0;
	unsigned long printlevel_off=0;
	unsigned int prev_level = current_level;
	unsigned int less_time = 0;
	LCD_init();
	InitADC();
	ld_init();
	TimerSet(150);
	TimerOn();
	printlevel();
    /* Insert your solution below */
    while (1){
	    DirectionTick();
	    MoveTick();
	    if(checkwin==true){
		less_time+=150;
		printlevel();
		checkwin=false;
		printlevel_off=less_time;
	    }
	    if(blinkwin_on >= 600 && checkwin==false){
		BlinkWinOn();
		blinkwin_on=0;
	    }
	    if(blinkwin_off >=1200 && checkwin==false){
		BlinkWinOff();
		blinkwin_off=0;
	    }
	    if(printlevel_off>=10000 && checkwin== false){
		printlevelOff();
	    }
	    while(!TimerFlag);
	    TimerFlag=0;
	    blinkwin_on+=timeperiod;
	    blinkwin_off+=timeperiod;
	    printlevel_off+=timeperiod;
    }
    return 1;
}
