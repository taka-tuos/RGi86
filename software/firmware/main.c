#include "xprintf.h"

unsigned char far *p;

void outc(unsigned char c) {
	*p++ = c;
}

int kernel_main(void)
{
	char *s = "hello,world";
	int i;
	
	p = (unsigned char far *)0xb8000000;
	
	xdev_out(outc);
	
	xprintf("hello,world %d",0);
	
	for(;;);
}
