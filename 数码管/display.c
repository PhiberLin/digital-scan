#include <reg52.h>
#include <intrins.h>

typedef unsigned char byte;

//********I/O�ڶ���********
sbit WE = P2^7;//λѡ��1Ϊ�������룬0Ϊ��������
sbit DU = P2^6;//��ѡ��ͬ��

//********��������********
void display(unsigned char *);
void clear(void);
void delay();		//@11.0592MHz
void send(unsigned char,unsigned char);

/**************************************
���ƣ�display
��������������Ͻ��ж�̬ɨ����ʾ�ַ�
������
num		unsigned char��ָ���������ָ����������Ϊ8���ַ�
			Ŀǰ����0123456789��������ʾ����*�����λ
			�����ַ�����ϡ�.�����������ַ����½ǡ�.��
ʾ����display("2016.06.25");
			display("******16");
			display("*03.**08.*");
**************************************/
void display(unsigned char * num)
{
	byte data i;
	byte xdata IO,datWe,offset;
	clear();
	datWe = 0xfe;
	offset = 0;//���ڡ�.����ɵ�ƫ������0
	for(i = 0;i < 8;i++)
	{
		switch(*(num+i+offset))//�ж�����ʾ�����֣�ȷ����ѡ��
		{
			case '0':	IO = 0x3F;	break;
			case '1':	IO = 0x06;	break;
			case '2':	IO = 0x5B;	break;
			case '3':	IO = 0x4F;	break;
			case '4':	IO = 0x66;	break;
			case '5':	IO = 0x6D;	break;
			case '6':	IO = 0x7D;	break;
			case '7':	IO = 0x07;	break;
			case '8':	IO = 0x7F;	break;
			case '9':	IO = 0x6F;	break;
			case '*': IO = 0x00; 	break;
			case 'A': IO = 0x77; 	break;
			case 'a': IO = 0x77; 	break;
			case 'B': IO = 0x7C; 	break;
			case 'b': IO = 0x7C; 	break;
			case 'C': IO = 0x39; 	break;
			case 'c': IO = 0x39; 	break;
			case 'D': IO = 0x5E; 	break;
			case 'd': IO = 0x5E; 	break;
			case 'E': IO = 0x79; 	break;
			case 'e': IO = 0x79; 	break;
			case 'F': IO = 0x71; 	break;
			case 'f': IO = 0x71; 	break;
			case 'H': IO = 0x76; 	break;
			case 'h': IO = 0x76; 	break;
			case 'i':	IO = 0x04;	break;
			case 'L':	IO = 0x38;	break;
			case 'N': IO = 0x37; 	break;
			case 'n': IO = 0x37; 	break;
			case 'U': IO = 0x3E; 	break;
			case 'u': IO = 0x3E; 	break;
			case 'O': IO = 0x5C; 	break;
			case 'o': IO = 0x5C; 	break;
			case 'P': IO = 0x73; 	break;
			case 'p': IO = 0x73; 	break;
			case 'R':	IO = 0x50;	break;
			case 'r':	IO = 0x50;	break;
			case '-': IO = 0x40; 	break;
			default:	IO = 0x00;	break;
		}
		if(*(num+i+offset+1) == '.')
		{
			IO += 0x80;
			offset++;
		}
		send(IO,datWe);
		clear();
		datWe = _crol_(datWe,1);
	}
}

/**************************************
���ƣ�displayInt
��������������Ͻ��ж�̬ɨ����ʾ����
������
numInt	����ʾ��������long int�ͱ����������ɸ���
				������-9999999~99999999֮�䣬������ʾError
ʾ����displayInt(2301);
			display(-9361523);
			display(22336666);
**************************************/

void displayInt(signed long int numInt)
{
	byte xdata str[] = "********";
	byte data i,flag;
	byte * pointer;
	long int num;
	pointer = str + 7;
	num = numInt;
	if(num>99999999 || num<-9999999)
	{
		display("error");
		return;
	}
	if(num<0)
	{
		flag = 1;
		num = -num;
	}
	for(i=0;i<8;i++)
	{
		byte chr;
		chr = num % 10;
		chr += '0';//������ת��ΪASCII��
		*pointer = chr;
		pointer--;
		num = num/10;
		if(num == 0)
			break;
	}
	if(flag == 1)
		*pointer = '-';
	display(str);
}

//����������ʾ
void clear()
{	
	P0 = 0x00;
	DU = 1;
	DU = 0;
	delay();
	P0 = 0xff;
	WE = 1;
	WE = 0;
}

//�����ʱ��ֻ�����������ʾ
void delay()		//@11.0592MHz
{
	unsigned char i, j;

	i = 2;
	j = 2;
	do
	{
		while (--j);
	} while (--i);
}



//��������������������ݣ��������뼰λ��
/*****************************************
������
value	����
position λ��
******************************************/
void send(unsigned char value,unsigned char position)
{
	P0 = value;
	DU = 1;
	DU = 0;
	P0 = position;
	WE = 1;
	WE = 0;
	delay();
	P0 = 0xff;
	WE = 1;
	WE = 0;
}
