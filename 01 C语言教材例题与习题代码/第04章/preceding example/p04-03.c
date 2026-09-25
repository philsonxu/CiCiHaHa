// 输入三个数，按由从小到大的顺序输出。

#include<stdio.h>
int main()
{
	float a, b, c, t;
	scanf("%f,%f,%f", &a, &b, &c);
	if (a > b)
	{
		t = a;
		a = b;
		b = t;
	}
	if (a > c)
	{
		t = a;
		a = c;
		c = t;
	}
	if (b > c)
	{
		t = b;
		b = c;
		c = t;
	}
	printf("%5.2f,%5.2f,%5.2f\n", a, b, c);
	return 0;
}

