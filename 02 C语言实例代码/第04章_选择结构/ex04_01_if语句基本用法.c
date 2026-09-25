/*
 * 例4.1 if语句基本用法 - 判断两个数的大小
 * 知识点：if单分支、关系运算符
 */
#include <stdio.h>

int main()
{
	int a, b;
	printf("请输入两个整数a,b：");
	scanf("%d,%d", &a, &b);

	if (a > b)
		printf("较大的数是：%d\n", a);
	if (a < b)
		printf("较大的数是：%d\n", b);
	if (a == b)
		printf("两个数相等：%d\n", a);

	return 0;
}