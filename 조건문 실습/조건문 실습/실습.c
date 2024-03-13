#include <stdio.h>
#include <conio.h>

int test01();
void test02();					//함수 선언(함수의 프로토타입)
void test03();
void test04();					//포인터를 이용한 문자열 입출력 함수(5일차)


//사용 함수 설정
void Dump(char* p, int len);	//메모리 공간 출력용 범용 함수
void Copy(char* p1, char* p2);


int main()
{
	//test01();
	//test02();
	//test03();
	test04();

}



int test01(void) {

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
void test02(void) {		//문자열과 문자배열
	char ca[] = "Hello";	//ca[0] = 'H', ..., ca[4] = 'o', ca[5] = null(\0)
	for (int i = 0; i < 6; i++) {
		printf("ca[%d]: %c (%02x) [%08x]\n", i, ca[i], ca[i], ca + i); //%02x: 2바이트씩 16진수로, [%08x]에는 주소 표현(ca + i)
	}

	int ia[] = { 10, 20, 30, 40, 50 };
	for (int i = 0; i < 6; i++) {
		printf("ia[%d]: %d (%08x) [%08x] \n", i, ia[i], ia[i], ia + i); //%08x: 8바이트씩 16진수로, [%08x]에는 주소 표현(ia + i)
	}

	int ia2[3][2] = { 10, 20, 30, 40, 50, 60 };
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 2; x++) {
			printf("ia2[%d][%d]: %d(해당 위치의 데이터) [%08x](해당 위치의 주소) \n", y, x, ia2[y][x], ia2 + y); //마지막 ia2+y에서 ia2는 배열 명 + y해줄 때 y가 1 증가할 때 2개의 int가 포함. 따라서 8바이트씩 증가
		}
	}



}
void test03() {
	char buf[100];			//메모리 공간에 방 100개 만들기(안전한 메모리 공간 확보)
	char* pBuf;				//안전 메모리 공간중의 출력 위치 설정
	unsigned int addr;				//메모리 공간 중 출력 위치 지정을 위한 입력 변수(주소)
	char kBuf[100];			//출력할 문자열을 scanf로 입력받기 위한 공간을 미리 확보

	printf("buf(안전공간)의 주소는 %d[%08x]입니다.\n", (unsigned int) buf);	//%d에서는 10진수로 표시되므로 [  ]안에 참고용으로 16진수 출력
	printf("입력을 시작할 주소를 입력하세요: ");//buf이름의 메모리 공간 중 값이 할당할 시작 주소 입력(위의 안전공간 주소 참고)
	scanf("%d", &addr);							//기존 주소에 addr만큼 더해진 주소로 입력 값 저장

	pBuf = buf + addr;							//위에서 초기화한 addr의 값을 pBuf에 저장

	printf("문자열을 입력하세요: ");
	scanf("%s", kBuf);				//출력할 문자열 입력
	Copy(pBuf, kBuf);				//strcpy(복사할 위치, 복사할 내용)

	Dump(buf, 100);					//길이는 kBuf에서 설정한 100
}
void test04() {
	char* arr[10] = { "aaaaa", "bbbb", "cccc", "dd", "eeeee" };	//각각의 문자열은 메모리 상단에 상수 영역에 저장, arr의 0, 1, 2, 3, 4에 저장
	char buf[100];

	printf("[6] ");
	scanf("%s", buf);
	arr[6] = buf; //입력받은 문자를 arr배열의 6에 저장

	printf("[7] ");
	scanf("%s", buf+50);
	arr[7] = buf+50; //입력받은 문자를 arr배열의 6에 저장


	for (int i = 0; i < 10; i++)
	{
		printf("arr[%d] : 0x%08x %s	\n",i, arr[i], arr[i]); //0x%08x에는 8자리의 16진수로 arr에 들어있는 값 표현
		//뒤의 두 arr[i]는 모두 주소값을 나타냄. 변환자에 의해 0x%08x에서는 주소 출력, s에서는 해당 값인 문자열 출력
	}
}



void Copy(char* p1, char* p2) {
	while (*p2) *p1++ = *p2++, *p1 = 0;;
	/*
	조건식:p2의 값(p2의 값이 null이 아니라면, 계속 수행)
	수행 내용: p1의 값에 p2값을 한글자씩 복사 => p2값의 마지막인 null값에 도달하면 p1값의 마지막에 null을 입력하며 완성
	*/
}
void Dump(char* p, int len) {							//p는 char 포인터(p는 주소값을 말함)
	for (int i = 0; i < len; i++) {						//함수 입력 시 입력받는 길이 len만큼만 반복
		if (i % 16 == 0)		printf("\n%08x", p);	//반복실행을 수행하기 위해 i가 16의 배수일 때 실행하도록 설정 -> 이 때 주소(p)를 출력
		if (i % 8 == 0)			printf("    ");			//8개를 출력할 때 공백 추가
		printf("%02x   ", (unsigned char)*p++);			//*의 의미: 포인터에서 p주소에 있는 값, 주소를 한칸씩 옮기며 출력
	}
}



//day04 아래줄에 넣기
//ca[2] -= 1;
//ca[3] -= 1;

//for (int i = 0; i < 6; i++) {
//	printf("ca[%d]: %c (%02x)\n", i, ca[i], ca[i]);
//}