// 输入两个整数，要求输出其中值较大者。 要求用函数来找到大数。

int max(int x, int y)
{
	int z;
	z = x > y ? x : y;
	return(z);
}
#include<stdio.h>
int main()
{
	int max(int x, int y);
	int a, b, c;
	printf("please enter two integer numbers:");
	scanf("%d,%d", &a, &b);
	c = max(a, b);
	printf("max is %d\n", c);
	return 0;
}

