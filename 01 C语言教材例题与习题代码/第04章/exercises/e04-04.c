#include<stdio.h>
/*
有3个整数a，b，c，由键盘输入，输出其中最大的数。
*/
int main()
{
	int a, b, c;
	printf("请输入3个整数:");
	scanf("%d,%d,%d", &a, &b, &c);
	if (a < b)
		if (b < c)
			printf("max=%d\n", c);
		else
			printf("max=%d\n", b);
	else if (a < c)
		printf("max=%d\n", c);
	else
		printf("max=%d\n", a);
	return 0;

}

