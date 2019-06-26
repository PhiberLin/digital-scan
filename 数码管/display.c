#include <reg52.h>
#include <intrins.h>

typedef unsigned char byte;

//********I/O口定义********
sbit WE = P2^7;//位选，1为数据输入，0为数据锁定
sbit DU = P2^6;//段选，同上

//********函数声明********
void display(unsigned char *);
void clear(void);
void delay();		//@11.0592MHz
void send(unsigned char,unsigned char);

/**************************************
名称：display
描述：在数码管上进行动态扫描显示字符
参数：
num		unsigned char型指针变量，所指向数组至少为8个字符
			目前包括0123456789，若无显示则用*代替空位
			可在字符后加上“.”来点亮该字符右下角“.”
示例：display("2016.06.25");
			display("******16");
			display("*03.**08.*");
**************************************/
void display(unsigned char * num)
{
	byte data i;
	byte xdata IO,datWe,offset;
	clear();
	datWe = 0xfe;
	offset = 0;//由于“.“造成的偏移量置0
	for(i = 0;i < 8;i++)
	{
		switch(*(num+i+offset))//判断所显示的数字，确定段选码
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
名称：displayInt
描述：在数码管上进行动态扫描显示整数
参数：
numInt	欲显示的整数，long int型变量，可正可负，
				必须在-9999999~99999999之间，否则显示Error
示例：displayInt(2301);
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
		chr += '0';//将数字转换为ASCII码
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

//清除数码管显示
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

//软件延时，只用于数码管显示
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



//向数码管锁存器传送数据，包括段码及位码
/*****************************************
参数：
value	段码
position 位码
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
