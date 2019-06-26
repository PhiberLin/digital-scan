#include <reg52.h>
#include <display.h>

typedef unsigned char byte;

//********全局变量********
byte code str[] = "error";

void main()
{
	while(1)
	{
		byte i=0;
		while(--i)
			display("bi1ibi1i");
		while(--i)
			display("2009.06.26");
	}
}