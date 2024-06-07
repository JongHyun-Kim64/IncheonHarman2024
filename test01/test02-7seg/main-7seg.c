/*
 * test02-7seg.c
 *
 * Created: 2024-03-21 오후 3:34:37
 * Author : SYSTEM-00
 */ 
#define F_CPU 16000000L					//Board CLK 정보(16MHz)
#include <avr/io.h>
#include <avr/delay.h>

uint8_t digit[] = {	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71 };
uint8_t character[] = { // -,
	0x40 };
	char arr[5];		//세그먼트 이미지 정보를 담을 안전공간
	
void LED(char*p, int n){//portp의 n번째 비트를 점멸
	char b = 1 << n;
	*p |= b;
	_delay_ms(500);
	*p &= ~b;
	_delay_ms(500);
}

void seg(int sel, uint8_t c){
	PORTC |= 0X0F;
	PORTC &= ~(1 << (3-sel));
	PORTD = c;	//숫자 데이터 출력
	_delay_ms(2);
}


void FND_4(char *inf){	//segment Image 배열
	for (int i = 0; i < 4; i++){
		seg(i, *(inf+i));
	}
}

//16진수 segment image 배열
char* Trans(unsigned long num){	//10진 정수를 입력받아 16진수 문자열로 변환 ex)65535 ==> 0xffff, 56506=>0xBCDA
	int n1 = num % 10;			//A(10): 문자가 아닌 숫자
	int n2 = (num / 10) % 10;	//B(11)
	int n3 = (num / 100) % 10;	//C(12)
	int n4 = num / 1000;		//D(13)
	
	arr[0] = digit[n1]; arr[1] = digit[n2]; arr[2] = digit[n3] + 0x80; arr[3] = digit[n4];
	
	if ( num<10 ){
		arr[3] = 0; arr[1] = 0; arr[2] = 0x80;
	}
	else if ( num<100 ){
		arr[2] = 0x80; arr[3] = 0;
	}
	else if ( num<1000 ){
		arr[3] = 0;
	}
	
	return arr;
}


int main(void)
{
	
	unsigned long i, j = 0;
	DDRD = 0xFF;						//세그먼트 제어 핀 8개를 출력으로 설정
	DDRC = 0x0F;						//자릿수 선택 핀 4개를 출력으로 설정
	DDRB &= ~0x01;						//B port의 0번째 비트는 입력용(0)으로, 나머지는 출력용(1)로 설정 (1111 1110 = NOT 0000 0001)
			
    //
    //while (1)							//한 번의 while loop 내에서 네자리 중 한 자리에 숫자를 번갈아가며 출력하는 코드
    //{
		//for (i = 0; i < 4; i++){		//해당 자리에만 0(GND)를 출력하고 나머지에는 1을 출력하여 자리 선택
			//PORTC |= 0X0f;
			//PORTC &= ~(1 << i);			//자리 선택
			//
			//for (j = 0; j < 16; j++){
				//PORTD = digit[j];
				//
				//_delay_ms(500);
			//}
		//}
    //}
//
	void display_digit(int position, int number){
		PORTC |= 0x0F;
		PORTC &= ~(1 << (position - 1));
	
		PORTD = digit[number];
	}
	void display_character(int position, int number){
		PORTC |= 0x0F;
		PORTC &= ~(1 << (position - 1));
	
		PORTD = character[number];
	}

	int mode = 0;

	//스탑워치
	while(1){
		if(mode >= 4) mode = 0;
	
		switch(mode){
			case 0:	//화면이 켜지면 mode0(---- 표시 코드)
				display_character(1, 0);
				_delay_ms(2);
				display_character(2, 0);
				_delay_ms(2);                                                                                                                                                                                                                                          
				display_character(3, 0);
				_delay_ms(2);
				display_character(4, 0);
				_delay_ms(2);
				if(!(PINB & 0x01 == 1)) mode = 1;
				break;
			case 1:	//버튼을 1회 누르면 스탑워치 작동 시작
				j = 0;
				while(1){
					FND_4(Trans(j++));
					_delay_ms(2);
					if(!(PINB & 0x01 == 1)) break; mode = 2;
				}
				break;
			case 2:	//버튼을 누르면 스탑워치 작동 정지
				_delay_ms(100);
				while(1){
					FND_4(Trans(j));
					_delay_ms(2);
					if(!(PINB & 0x01 == 1)) break; 
				}
				_delay_ms(300);
				mode = 0;
				break;
		}
	}
}

