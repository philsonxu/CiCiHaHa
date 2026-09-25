// 从键盘输入BOY三个字符，然后把它们输出到屏幕。 

#include<stdio.h> 
int main()
{
	char a, b, c;
	a = getchar();
	b = getchar();
	c = getchar();
	putchar(a);
	putchar(b);
	putchar(c);
	putchar('\n');
	return 0;
}

