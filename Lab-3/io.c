#typedef unsigned char u8;
#typedef unsigned short u16;
#typedef unsigned long u32;

char *chars = "0123456789ABCDEF";



int rpu(int x){
	char c;
	if(x){
		c = chars[x%base];
		rpu(x/base);
		putc(c);
	}
}
int printu(int x){
	if(x=='0')
		putc('0');
	else
		rpu(x);
}
int printd(int x){
	if(x=='0')
		putc('0');
	else if(x<0){
		putc('-');
		rpu(-1*x);
	}
	else
		rpu(x);
}

int prints(char *s)
{
   while(*s!='\0'){
	putc(*s);
	s++;}
	return 0;
}

void printf(char *fmt, ...){
	char *tmp = fmt;
	u16 *first = (int*)&fmt +1;
	u32 *lng;
}


