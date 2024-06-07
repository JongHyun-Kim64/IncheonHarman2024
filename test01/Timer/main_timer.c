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
#define SETMAX 2
#define RESETMAX 2

uint8_t digit[] = {	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71 };
char arr[5];		//세그먼트 이미지 정보를 담을 안전공간
volatile int opt = 0, set = 0, reset = 0;


void seg(int sel, uint8_t c){
	PORTC |= 0X0F;
	PORTC &= ~(1 << (3-sel));
	PORTD = c;	//숫자 데이터 출력
	_delay_ms(2.5);
}


void FND_4(char *inf){	//segment Image 배열
	for (int i = 0; i < 4; i++){
		seg(i, *(inf+i));

	}
}

//16진수 segment image 배열
char* Display(unsigned long num){	//10진 정수를 입력받아 16진수 문자열로 변환 ex)65535 ==> 0xffff, 56506=>0xBCDA
	int n1 = (num / 100) % 10;			//A(10): 문자가 아닌 숫자
	int n2 = ((num / 1000) % 10)%6;	//B(11)
	int n3 = (num / 6000) % 10;	//C(12)
	int n4 = (num / 60000) % 10;		//D(13)
	
	arr[0] = digit[n1]; arr[1] = digit[n2]; arr[2] = digit[n3] + 0x80; arr[3] = digit[n4];
	
	//if ( num<10 ){
		//arr[3] = 0; arr[1] = 0; arr[2] = 0x80;
	//}
	//else if ( num<100 ){
		//arr[2] = 0x80; arr[3] = 0;
	//}
	//else if ( num<1000 ){
		//arr[3] = 0;
	//}
	//
	FND_4(arr);
	return arr;
}



int main(void)
{
	//7-Segment 사용	: 4 Module - C type
	//	Pin assign	: PDx - Segment img, PCx - module sel
	//Interrupt 사용	: INT4~INT6 (External Interrupt)
	//	Pin assign	: PE4~PE6
	DDRA |= 0x80;	//PA의 포트7을 출력으로 사용 1000 0000
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
		switch(set){
			case 0:	//시간 0 표시
				t = 0;
				break;
			case 1:	//stopwatch start
				t++;
				break;
			case 2:	//stopwatch stop
				break;
			default:  break;
		}
		switch(opt){
			case 0:	//reset단계
				t = 0; 
				break;
			case 1:	//stopwatch start
				t++;
				break;
			case 2:	//stopwatch stop
				break;
			default:  break;
	}
	Display(t);
		//set이 0이면 시간 0, 1이면 1초에 10초씩 증가, state가 2가 되면 증가를 멈추고 시간 저장

		
	}
}

ISR(INT4_vect){	//INT4 인터럽트 처리 루틴: sw1
	opt++;
	if (opt >= OPTMAX) opt = 1;
}

ISR(INT5_vect){	//INT5 인터럽트 처리 루틴: sw2
	set++;
	if (set >= SETMAX) set = 0;
}

ISR(INT6_vect){	//INT6 인터럽트 처리 루틴: sw3
	
}