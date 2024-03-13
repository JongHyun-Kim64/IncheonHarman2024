#include <stdio.h>
#include <conio.h>

int test01();
void test02();					//�Լ� ����(�Լ��� ������Ÿ��)
void test03();
void test04();					//�����͸� �̿��� ���ڿ� ����� �Լ�(5����)


//��� �Լ� ����
void Dump(char* p, int len);	//�޸� ���� ��¿� ���� �Լ�
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
void test02(void) {		//���ڿ��� ���ڹ迭
	char ca[] = "Hello";	//ca[0] = 'H', ..., ca[4] = 'o', ca[5] = null(\0)
	for (int i = 0; i < 6; i++) {
		printf("ca[%d]: %c (%02x) [%08x]\n", i, ca[i], ca[i], ca + i); //%02x: 2����Ʈ�� 16������, [%08x]���� �ּ� ǥ��(ca + i)
	}

	int ia[] = { 10, 20, 30, 40, 50 };
	for (int i = 0; i < 6; i++) {
		printf("ia[%d]: %d (%08x) [%08x] \n", i, ia[i], ia[i], ia + i); //%08x: 8����Ʈ�� 16������, [%08x]���� �ּ� ǥ��(ia + i)
	}

	int ia2[3][2] = { 10, 20, 30, 40, 50, 60 };
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 2; x++) {
			printf("ia2[%d][%d]: %d(�ش� ��ġ�� ������) [%08x](�ش� ��ġ�� �ּ�) \n", y, x, ia2[y][x], ia2 + y); //������ ia2+y���� ia2�� �迭 �� + y���� �� y�� 1 ������ �� 2���� int�� ����. ���� 8����Ʈ�� ����
		}
	}



}
void test03() {
	char buf[100];			//�޸� ������ �� 100�� �����(������ �޸� ���� Ȯ��)
	char* pBuf;				//���� �޸� �������� ��� ��ġ ����
	unsigned int addr;				//�޸� ���� �� ��� ��ġ ������ ���� �Է� ����(�ּ�)
	char kBuf[100];			//����� ���ڿ��� scanf�� �Է¹ޱ� ���� ������ �̸� Ȯ��

	printf("buf(��������)�� �ּҴ� %d[%08x]�Դϴ�.\n", (unsigned int) buf);	//%d������ 10������ ǥ�õǹǷ� [  ]�ȿ� ��������� 16���� ���
	printf("�Է��� ������ �ּҸ� �Է��ϼ���: ");//buf�̸��� �޸� ���� �� ���� �Ҵ��� ���� �ּ� �Է�(���� �������� �ּ� ����)
	scanf("%d", &addr);							//���� �ּҿ� addr��ŭ ������ �ּҷ� �Է� �� ����

	pBuf = buf + addr;							//������ �ʱ�ȭ�� addr�� ���� pBuf�� ����

	printf("���ڿ��� �Է��ϼ���: ");
	scanf("%s", kBuf);				//����� ���ڿ� �Է�
	Copy(pBuf, kBuf);				//strcpy(������ ��ġ, ������ ����)

	Dump(buf, 100);					//���̴� kBuf���� ������ 100
}
void test04() {
	char* arr[10] = { "aaaaa", "bbbb", "cccc", "dd", "eeeee" };	//������ ���ڿ��� �޸� ��ܿ� ��� ������ ����, arr�� 0, 1, 2, 3, 4�� ����
	char buf[100];

	printf("[6] ");
	scanf("%s", buf);
	arr[6] = buf; //�Է¹��� ���ڸ� arr�迭�� 6�� ����

	printf("[7] ");
	scanf("%s", buf+50);
	arr[7] = buf+50; //�Է¹��� ���ڸ� arr�迭�� 6�� ����


	for (int i = 0; i < 10; i++)
	{
		printf("arr[%d] : 0x%08x %s	\n",i, arr[i], arr[i]); //0x%08x���� 8�ڸ��� 16������ arr�� ����ִ� �� ǥ��
		//���� �� arr[i]�� ��� �ּҰ��� ��Ÿ��. ��ȯ�ڿ� ���� 0x%08x������ �ּ� ���, s������ �ش� ���� ���ڿ� ���
	}
}



void Copy(char* p1, char* p2) {
	while (*p2) *p1++ = *p2++, *p1 = 0;;
	/*
	���ǽ�:p2�� ��(p2�� ���� null�� �ƴ϶��, ��� ����)
	���� ����: p1�� ���� p2���� �ѱ��ھ� ���� => p2���� �������� null���� �����ϸ� p1���� �������� null�� �Է��ϸ� �ϼ�
	*/
}
void Dump(char* p, int len) {							//p�� char ������(p�� �ּҰ��� ����)
	for (int i = 0; i < len; i++) {						//�Լ� �Է� �� �Է¹޴� ���� len��ŭ�� �ݺ�
		if (i % 16 == 0)		printf("\n%08x", p);	//�ݺ������� �����ϱ� ���� i�� 16�� ����� �� �����ϵ��� ���� -> �� �� �ּ�(p)�� ���
		if (i % 8 == 0)			printf("    ");			//8���� ����� �� ���� �߰�
		printf("%02x   ", (unsigned char)*p++);			//*�� �ǹ�: �����Ϳ��� p�ּҿ� �ִ� ��, �ּҸ� ��ĭ�� �ű�� ���
	}
}



//day04 �Ʒ��ٿ� �ֱ�
//ca[2] -= 1;
//ca[3] -= 1;

//for (int i = 0; i < 6; i++) {
//	printf("ca[%d]: %c (%02x)\n", i, ca[i], ca[i]);
//}