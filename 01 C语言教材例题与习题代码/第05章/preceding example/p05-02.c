// 用do while 求1+2+3+...+100，即\sum_{n=1}^{100}n

#include<stdio.h>
int main()
{
	int i = 1, sum = 0;
	do
	{
		sum = sum + i;
		i++;
	} while (i <= 100);
	printf("sum=%d\n", sum);	// 输出1+2+3+...+100的累加和 
	return 0;
}

