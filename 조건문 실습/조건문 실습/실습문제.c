#include <stdio.h>
#include <conio.h>

//테스트

int day03();
void day04();	//함수 선언(함수의 프로토타입)

int day03(void) {

	char* str[] = { "Zero", "One", "Two", "Three", "Four",
		"Five", "Six", "Seven", "Eight", "Nine" }; // 문자열 포인터(*) 배열 선언

	printf("숫자키를 입력하면 해당 영단어를 표시합니다.(x는 종료)\n");
	while (1) {
		printf(">");
		char c = getch();
		printf("%c\n", c);
		
		if (c == 'x' || c == 'X') break;//c | 0x20 == 'x'로 조건 넣어도 작동. c | 0x20을 통해 입력 값을 소문자로 변환

		int m = c - 0x30; //ASCII -> num값
		printf("%c: %s\n", c, str[m]); //입력 값c: str의 m번째 값.

		/*switch (c) {
		case '1':	printf("One\n");	break;
		case '2':	printf("Two\n");	break;
		case '3':	printf("Three\n");	break;
		case '4':	printf("Four\n");	break;
		case '5':	printf("Five\n");	break;
		case '6':	printf("Six\n");	break;
		case '7':	printf("Seven\n");	break;
		case '8':	printf("Eight\n");	break;
		case '9':	printf("Nine\n");	break;
		case '0':	printf("Zero\n");	break;
		default:	printf("숫자가 아닙니다.\n");	break;
		}*/
		printf("\n");
	}
}

void day04(void) {		//문자열과 문자배열
	char ca[] = "Hello";	//ca[0] = 'H', ..., ca[4] = 'o', ca[5] = null(\0)
	for (int i = 0; i < 6; i++) {
		printf("ca[%d]: %c (%02x)\n", i, ca[i], ca[i]);
	}

	ca[2] -= 1;
	ca[3] -= 1;

	for (int i = 0; i < 6; i++) {
		printf("ca[%d]: %c (%02x)\n", i, ca[i], ca[i]);
	}
}

int main()
{
	//day03();
	day04();
}
