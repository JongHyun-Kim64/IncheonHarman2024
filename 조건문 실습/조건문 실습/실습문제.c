#include <stdio.h>
#include <conio.h>

int day03();
void day04();	//�Լ� ����(�Լ��� ������Ÿ��)

int day03(void) {

	char* str[] = { "Zero", "One", "Two", "Three", "Four",
		"Five", "Six", "Seven", "Eight", "Nine" }; // ���ڿ� ������(*) �迭 ����

	printf("����Ű�� �Է��ϸ� �ش� ���ܾ ǥ���մϴ�.(x�� ����)\n");
	while (1) {
		printf(">");
		char c = getch();
		printf("%c\n", c);
		
		if (c == 'x' || c == 'X') break;//c | 0x20 == 'x'�� ���� �־ �۵�. c | 0x20�� ���� �Է� ���� �ҹ��ڷ� ��ȯ

		int m = c - 0x30; //ASCII -> num��
		printf("%c: %s\n", c, str[m]); //�Է� ��c: str�� m��° ��.

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
		default:	printf("���ڰ� �ƴմϴ�.\n");	break;
		}*/
		printf("\n");
	}
}

void day04(void) {		//���ڿ��� ���ڹ迭
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