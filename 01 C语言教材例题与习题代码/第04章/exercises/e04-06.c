/*有一个函数∶
y=x(x<1)
y=2x-1 (1≤x<10)
y=3x-11(x≥10)
写程序，输入x的值，输出y相应的值。
*/

#include<stdio.h>
int main()
{
	int x, y;
	printf("输入x:");
	scanf("%d", &x);
	if (x < 1)
	{
		y = x;
		printf("x=%3d,	y=x=%d\n", x, y);
	}
	else if (x < 10)
	{
		y = 2 * x - 1;
		printf("x=%3d,	y=2*x-1=%d\n", x, y);
	}
	else
	{
		y = 3 * x - 11;
		printf("x=%3d,	y=3*x-11=%d\n", x, y);
	}
	return 0;
}

