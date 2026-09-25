// 求两个整数中较大者
// 思路：用一个函数来实现求两个整数中的较大者。在主函数中调用此函数并输出结果。
#include<stdio.h> 
int main()
{
	int max(int x, int y);// 对被调用函数max的声明
	int a, b, c;
	scanf("%d,%d", &a, &b);
	c = max(a, b);
	printf("max=%d\n", c);
	return 0;
}

// 求两个整数中的较大者的max函数
int max(int x, int y)
{
	int z;
	if (x > y)z = x;
	else z = y;
	return(z);
}

