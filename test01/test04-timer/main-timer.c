/*
 * test04-timer.c
 *
 * Created: 2024-03-28 오전 10:57:17
 * Author : jong9
 */ 

#include "myHeader.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#define _delay_t 500
/*
opt: 타이머 작동 시 start/stop/continue 기능을 순환하기 위한 변수
settimeth: Set Time (th) -> 세그먼트의 몇 번째 자리에 시간을 표시할 지 결정하는 변수
reset; sw3으로 reset동작을 실행할 때 이용하는 변수
timeup: 초기 시간 설정 시 증가시키는 시간의 값을 저장하기 위한 변수
timedown: 초기 시간 설정 시 감소시키는 시간의 값을 저장하기 위한 변수
complete: 시간 설정 시 설정을 저장하고 다음 자릿수로 넘어가기 위한 변수
*/
volatile int opt=0, settimeth=0, reset=0, timeup = 0,timedown = 0, complete = 0;
volatile unsigned long cnt = 0, tcnt = 0;
uint8_t	numb[] = {	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71 };	//0~9까지 세그먼트로 표시할 수 있는 led제어값
int mode = 0, endtime = 0;

char timearr[4] = {0x40, 0x40, 0xC0, 0x40};	//timesarr는 설정 값은 숫자로, 미설정 값은 -로 세그먼트에 표시하기 위한 값이 할당된 배열
char timeinit[4] = {0x40, 0x40, 0xC0, 0x40};//timesinit은 설정된 값은 깜빡이지 않도록, 설정되지 않은 값은 깜빡이도록 값이 할당된 배열
	
int main(void)
{	
	//기본 설정 묶음
	{
	SegPortSet(&PORTD, &PORTC);
	/*7-Segment 사용	: 4 Module - C type
	Pin assign	: PDx - Segment img, PCx - module sel
	Interrupt 사용	: INT4~INT6 (External Interrupt)
	Pin assign	: PE4~PE6
	*/

	//인터럽트 설정
	EIMSK = 0x70;							//0111 0000	//INT 4~INT 6 활성화
	EICRB = 0x2a;							//4개의 B그룹(INT4~INT7)의 인터럽트 발생 시점 결정(00 10 10 10, 각 7 6 5 4에서의 INT발생 시점을 rising edge로 결정)
	DDRD = 0XFF;							//D포트의 모든 비트를 출력으로 설정
	DDRC = 0X0F;							//D포트의 네개 비트를 출력으로 설정
	DDRE = 0x00;
	DDRA |= 0x80;							//A포트의 7번 비트를 출력으로 설정
	
	TIMSK |= 0x04;							//Timer Interrupt Mask을 0000 0100b로 설정하여 오버플로 인터럽트 활성화 [Timer 1 TCNT Overflow interrupt] 
    //TIMSK |= 0x01;						//Timer Interrupt Mask을 0000 0001b로 설정하여 오버플로 인터럽트 활성화 [Timer 0 TCNT Overflow interrupt] 
	//TIMSK |= 0x40;						//Timer Interrupt Mask을 0000 0001b로 설정하여 오버플로 인터럽트 활성화 [Timer 2 TCNT Overflow interrupt] 
	TCCR1B |= 0x04;							//16KHz를 구하기 위해 분주비 (Pre-Scaler) 64 필요 -> 비트를 0000 0100b로 설정 (타이머 1에 대한 분주기가 256)
	//TCCR0 |= 0x04;						//16KHz를 구하기 위해 분주비 (Pre-Scaler) 64 필요 -> 비트를 0000 0100b로 설정 (타이머 0에 대한 분주기)
	//TCCR2 |= 0x04; 						//16KHz를 구하기 위해 분주비 (Pre-Scaler) 64 필요 -> 비트를 0000 0100b로 설정 (타이머 2에 대한 분주기)
	SREG |= 0x80;
	sei();
	}
	
	int time;	
	int min10, min1, sec10, sec1;

	//timesarr배열과 timesinit배열이 번갈아 표시되며 숫자의 깜빡임을 제어하는 함수
	void blinknum(void){					
			for (int t = 0; t < 50; t++){
				FND_4(timearr);				
				_delay_ms(1);
			}
			_delay_ms(10);
			for (int t = 0; t < 50; t++){
				FND_4(timeinit);			
				_delay_ms(1);
			}
			_delay_ms(10);
	}
	
    while (1) 
    {				
		switch (mode){														//처음 시작하면 mode가 0이므로 초기 시간 설정화면 코드
			case 0:															//처음 시간 설정 모드
				switch(settimeth){
					case 0:													//10분 단위 설정
						timeup = 0; timedown = 0;							//up,down 버튼 눌린 횟수 0으로 초기화
						timeinit[3] = 0;									//선택 자릿수 깜빡이도록 0으로 배열 설정
						while(1){
							time = timeup - timedown;
						
							if(timeup == 0){								//timeup이 0이면 버튼을 누르지 않았으므로 - 표시
								timearr[3] = numb[time];
								 blinknum();
							}
							else{											//timeup이 0이 아니면 버튼을 눌렀으므로 화면에 숫자 표시
								if ( time > 9) time = 9;					//10분 단위는 9이상 올라갈 수 없으므로 최댓값 9로 설정
								if ( time <= 0) time = 0;					//시간은 0 이하 내려갈 수 없으므로 최솟값 0로 설정
							
								timearr[3] = numb[time];
								blinknum();
								if(complete == 1){							//sw3을 누르면 탈출, 다음 숫자로 넘김
									timearr[3] = numb[time];
									_delay_ms(10);
									min10 = time;
									break;
								}
							}
					}
					_delay_ms(100);
					complete = 0;
					settimeth = 1;
					break;
					
					
					case 1:	//1분 단위 설정
						timeup = 0; timedown = 0;
						timeinit[2] = 0x80;									//선택 자릿수 깜빡이도록 0으로 배열 설정
						timeinit[3] = numb[time];
						while(1){
							time = timeup - timedown;
						
							if(timeup == 0){								//timeup이 0이면 버튼을 누르지 않았으므로 - 표시
								timearr[2] = numb[time]+0x80;
								blinknum();
							}
							else{											//timeup이 0이 아니면 버튼을 눌렀으므로 화면에 숫자 표시
								if ( time > 9) time = 9;					//10분 단위는 9이상 올라갈 수 없으므로 최댓값 9로 설정
								if ( time <= 0) time = 0;					//시간은 0 이하 내려갈 수 없으므로 최솟값 0로 설정
							
								timearr[2] = numb[time]+0x80;
								blinknum();
								if(complete == 1){							//sw3을 누르면 탈출, 다음 숫자로 넘김
									timearr[2] = numb[time]+0x80;
									_delay_ms(10);
									min1 = time;
									break;
								}
							}
						}
						_delay_ms(100);
						complete = 0;
						settimeth = 2;
					break;
					
					
					case 2:	//10초 단위 설정
					complete = 0;
					timeinit[2] = numb[time]+0x80;
					timeinit[1] = 0;
					//timeinit[3-settimeth] = 0;							//선택 자릿수 깜빡이도록 배열 설정
					timeup = 0; timedown = 0;
					while(1){
						time = timeup - timedown;
						if(timeup == 0){									//timeup이 0이면 버튼을 누르지 않았으므로 - 표시
							timearr[1] = numb[time];
							blinknum();
						}
						else{												//timeup이 0이 아니면 버튼을 눌렀으므로 화면에 숫자 표시
							if ( time > 5) time = 5;						//10초 단위는 6이상 올라갈 수 없으므로 최댓값 5로 설정
							if ( time <= 0) time = 0;						//시간은 0 이하 내려갈 수 없으므로 최솟값 0로 설정
							timearr[1] = numb[time];
							blinknum();
									
							if(complete == 1){								//sw3을 누르면 탈출, 다음 숫자로 넘김
								timearr[1] = numb[time];
								_delay_ms(10);
								sec10 = time;
								break;
							}
						}
					}
					_delay_ms(100);
					complete = 0;
					settimeth = 3;
					break;
					
					case 3://1초 단위 설정
					timeinit[0] = 0;										//선택 자릿수 깜빡이도록 배열 설정
					complete = 0;
					timeinit[1] = numb[time];
					timeup = 0; timedown = 0;
					while(1){
						time = timeup - timedown;
						if(timeup == 0){									//timeup이 0이면 버튼을 누르지 않았으므로 - 표시
							timearr[0] = numb[time];
							blinknum();
						}
						else{												//timeup이 0이 아니면 버튼을 눌렀으므로 화면에 숫자 표시
							if ( time > 9) time = 9;						//10분 단위는 9이상 올라갈 수 없으므로 최댓값 9로 설정
							if ( time <= 0) time = 0;						//시간은 0 이하 내려갈 수 없으므로 최솟값 0로 설정
							
							timearr[0] = numb[time];
							blinknum();
							if(complete == 1){								//sw3을 누르면 탈출, 다음 숫자로 넘김
								timearr[0] = numb[time];
								_delay_ms(10);
								sec1 = time;
								break;
							}
						}
					}
					_delay_ms(100);
					complete = 0;
					mode = 1;
					break;
				}
				break;//시간 설정 종료
					
			//시간 설정 종료 후 sw1을 누르면 타이머 작동 시작		
			case 1:
				opt = 0; reset = 0; timedown = 0;
				endtime = min10 * 600 + min1 * 60 + sec10 * 10 + sec1;	//끝나는 시간이 cnt와 맞을 때 종료하도록 값 설정
				
				if (opt == 0) {											//타이머를 시작하는 sw1을 누르기 전에는 설정 시간 표시
					while(opt == 0){
					FND_4(timearr);
					}
				}
				while(cnt != endtime){									//sw1을 눌러 start 기능 작동
						if(cnt > 0x10000) cnt = 0;	//
						Display(endtime - cnt);
						if (timedown != 0){								//reset버튼을 누르면 모두 초기화 후 초기 화면으로 복귀
					opt=0, settimeth=0, reset=0, timeup = 0,timedown = 0, complete = 0, endtime = 0; break;
						}
				}
				
				
				if(cnt == endtime){										//시간이 맞으면 LED 점멸, RESET을 누르면
					reset = 0;
					while(reset == 0){									//reset이 눌리면 끝나도록
						PORTA |= 0x80;									//Port A의 7번째 비트를 1(HIGH)로 설정
						_delay_ms(500);
						PORTA &= ~0x80;									//Port A의 7번째 비트를 0(LOW)로 설정
						_delay_ms(500);
					}
					opt=0, settimeth=0, reset=0, timeup = 0,timedown = 0, complete = 0, endtime = 0;
				}
				//종료되면 모든 값 초기화 후 복귀
				for(int init = 0; init < 4; init ++){
					timearr[init]=0x40;
					timeinit[init]=0x40;
				}
				timearr[2]=0xC0;
				timeinit[2]=0xC0;
				mode = 0;
				break;//타이머 작동 종료
			}//switch mode
		}	//main의 while
	}	//main끝


ISR(INT4_vect){	//INT4 인터럽트 처리 루틴: sw1 (start, pause, continue 버튼, 기능은 opt이 결정)
	timeup++;
	opt++;
	if (opt >= 2) opt = 0;
}

ISR(INT5_vect){	//INT5 인터럽트 처리 루틴: sw2 (시간 설정 버튼)
	timedown++;
}

ISR(INT6_vect){	//INT6 인터럽트 처리 루틴: sw3 (reset 버튼)
	
	cnt = 0; opt = 0; complete = 1; settimeth++; reset = 1;
	if (settimeth > 3) mode = 1; opt = 0;
}

ISR(TIMER1_OVF_vect){
		tcnt++;
		if(mode == 1 & opt ==1){// sw1을 누르면 opt값이 변하므로 일시정지 구현 가능
		//if (tcnt >= 1000){
			cnt++; tcnt = 0;//1초(1000밀리초)가 지나면 cnt값을 증가시켜라
		//}
		}
}
