#include <reg52.h>
#include <display.h>


//********全局变量********
unsigned char code str[] = "error";

void main()
{
	display(IntToStr(3202));
}