/*
 * ADC.c
 *
 * Created: 2024-05-31 오후 4:33:58
 * Author : user
 */ 

#include <avr/io.h>
#include "myHeader.h"
#include <avr/delay.h>
#include <avr/interrupt.h>

int cint = 0; tcnt = 0;

void initADC(int ch){
	ADMUX |= (1 << REFS0);	//AVCC를 기준 전압으로 선택하는 코드. REFSn이 기준 전압 설정. <<는 shift 의미
	
	ADCSRA |= 0x07;			//분주비 설정(하위 3비트)
	ADCSRA |= 1 << ADEN;	//ADC 활성화
	ADCSRA |= 1 << ADFR;	//Free-Running mode 설정
	
	ADMUX = (ADMUX & 0xE0) | ch;
	ADCSRA |= 1 << ADSC;
	
}

int main(void)
{
	SegPortSet(&PORTC, &PORTD);
    initADC(0);	//1: ch number
    while (1) 
    {
		while(!(ADCSRA & (1<<ADIF))); //ADCStateResistor이 읽을 준비가 되지 않았다면 기다려라
		int cnt = ADC;
		Display(cnt);
    }
}

