#define F_CPU 16000000L				//Board CLK 정보(16MHz)

#include <avr/io.h>
#include <avr/delay.h>				//delay를 실행하기 위한 헤더파일

void togglepinA(int n){				//n: n번째 비트, dl: delay in mili-second
	char b = 1 << n;				//n번만큼 비트를 왼쪽으로 이동하라 (n이 1이면 10이 되어 2, n이 2이면 100이 되어 4 표현)
	PORTA |= b;						//위의 식을 따라 b번째 비트를 HIGH로 출력
	_delay_ms(500);
	PORTA &= ~b;					//Port A의 최하위비트(0번째 비트)를 0(LOW)로 설정}
	_delay_ms(500);
}

int main(void)
{
	DDRA |= 0x07;					//A port의 0번째 비트(PA0), 1번째 비트, 2번째 비트를 출력용(1)으로 사용(0000 0011), 나머지는 입력용(0)
	DDRB &= ~0x01;					//B port의 0번째 비트는 입력용(0)으로, 나머지는 출력용(1)로 설정 (1111 1110 = NOT 0000 0001)

	int mode = 0; //mode0 Green, mode1 Yellow, mode2 Red LED 점멸
	
	while (1)
	{
		
		
		if(!(PINB & 0x01 == 1)){		//B port의 0번 비트의 값이 0이라면(스위치가 눌리면)
			int on = 1;
			while(on == 1){
				togglepinA(mode);			//0번째 비트(LED)를 점멸
				if(!(PINB & 0x01 == 1)) on = 0;
			}
			mode++;
			if (mode>2) mode=0;			//2번째 LED 점멸 후 다시 0번째부터 실행해야 하므로 i 초기화
		}
	}
}