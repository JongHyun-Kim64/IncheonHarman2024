/*
 * test06-UART.c
 *
 * Created: 2024-06-07 오전 10:36:22
 * Author : user
 */ 

#include <avr/io.h>
#include <avr/delay.h>

void initUART0(){//BaudRate 설정 9600N81
	UBRR0H = 0; //상위 UBRRn 비트는 0
	UBRR0L = 207;	//하위 UBRRn 비트를 207로 설정 -> Baud Rate : 9600
	
	UCSR0A |= (1 << 1);	//0000 0010b 2배속 설정
	
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0); //RX, TX Enable
	UCSR0C |= 0x06;	//Data Bit : Default 8 (x11x)	0000 0100b N81
					//Stop Bit : 1 0xxx
	
}

void uPutc(char c){//설정된 포트를 이용하여 1Char을 send
// 	while ( 1 ){
// 		if(UCSR0A & ( 1<<UDRE0 ) == 1) break;	//wait until data empty bit = 1
// 	};	
	while(!(UCSR0A & (1<<UDRE0)));	//wait until data empty bit = 1
	UDR0 = c;	//send 
}

void uPuts(char *str){//설정된 포트를 이용하여 1string을 send
	while(*str) uPutc(*str++);	//str길이만큼 str의 한글자씩 출력\
	
	
// 	while(1){
// 		if(*str == 0) break;
// 		uPutc(*str);
// 		str++;
//	}
}
unsigned char buf[1024];
void bPrint(){ //고정된 메모리 공간 buffer 내의 문자열 촐력
	uPuts(buf);
}

int main(void)
{
    initUART0();	//Initialize
	char a = '0';
	int i = 0;
	uPuts("안녕하세요\r\n");
    while (1) {
		//uPutc(a++);	//보낼때마다 a를 증가시켜 0부터 증가하며 데이터 전송
		sprintf(buf, "ATmega128 터미널 출력 테스트... #%d\r\n", i++);	bPrint();//buf공간 내의 값을 i를 증가시키며 하나식 출력
		_delay_ms(1000);	//1000milli(=1) second delay
		//if(a >= '9') a = '0';	//a가 0~9를 반복
    }
}

