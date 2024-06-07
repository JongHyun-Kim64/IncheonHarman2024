/*
* test03.c
*
* Created: 2024-03-27 오후 12:26:02
* Author : jong9
*/
#define F_CPU 16000000L					//Board CLK 정보(16MHz)
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#define OPTMAX 3
#define STATEMAX 3

extern char* Display (unsigned long num);

volatile int opt = 0, state = 0;

int main(void)
{
	//7-Segment 사용	: 4 Module - C type
	//	Pin assign	: PDx - Segment img, PCx - module sel
	//Interrupt 사용	: INT4~INT6 (External Interrupt)
	//	Pin assign	: PE4~PE6
	DDRD = 0xFF;
	DDRC = 0x0F;
	DDRE = 0x00;
	
	//인터럽트 설정
	EIMSK = 0x70;	//0111 0000	//INT 4~INT 6 활성화
	EICRB = 0x2a;	//4개의 B그룹(INT4~INT7)의 인터럽트 발생 시점 결정(00 10 10 10, 각 7 6 5 4에서의 INT발생 시점을 rising edge로 결정)
	SREG |= 0x80;	//status Register - 인터럽트 허용 상태 레지스터
	sei();			//set interrupt
	
	int t = 0;
	while (1)
	{
		switch(opt){
			case 0:	//reset단계
				t = 0; break;
			case 1:	//stopwatch start
				t++; break;
			case 2:	//stopwatch stop
				break;
			default:  break;
		}
		Display(t);	//값이 증가하며 Disp가 호출될 때 t가 증가하며 각 자릿수에 맞는 시간 표시		
		_delay_ms(2);
	}
}

ISR(INT4_vect){	//INT4 인터럽트 처리 루틴: sw1
	opt++;
	if (opt >= OPTMAX) opt = 0;
}

ISR(INT5_vect){	//INT5 인터럽트 처리 루틴: sw2
	state++;
	if (state >= STATEMAX) state = 0;
}

ISR(INT6_vect){	//INT6 인터럽트 처리 루틴: sw3
	
}