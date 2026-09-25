#include<stdio.h>

int main()
{
	int a, b, c, max;
	printf("please input a,b,c：\n");
	scanf("%d%d%d", &a, &b, &c);
	max = a;
	if (max < b)
		max = b;
	if (max < c)
		max = c;
	printf("三个数中最大的为：%d", max);
	return 0;

}

