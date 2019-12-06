int kernel_main(void)
{
	char *s = "Hello,World!";
	int i;
	
	for(i=0;s[i];i++) ((unsigned short far *)0xb8000000)[i] = s[i] | (0x07 << 8);
	
	for(;;);
}
