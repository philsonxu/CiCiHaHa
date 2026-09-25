// do while 和 while 的比较

// （1）用while循环
#include<stdio.h>
int main()
{
	int i, sum = 0;
	printf("please enter i,i=?");
	scanf("%d", &i);
	while (i < 10)
	{
		sum = sum + i;
		i++;
	}
	printf("sum=%d\n", sum);
	return 0;
}

// 运行两次
//1
//11 

