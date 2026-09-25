/*
将一个数组中的值按逆序重新存放。例如，原来顺序为8，6，5，4，1。
要求改为1，4，5，6，8。

*/

#include<stdio.h> 
#define N 5
int main()
{
	int a[N], i, temp;
	printf("enter array a:\n");
	for (i = 0; i < N; i++)
	{
		scanf("%d", &a[i]);
	}
	printf("array a:\n");
	for (i = 0; i < N; i++)
	{
		printf("%4d", a[i]);
	}
	for (i = 0; i < N / 2; i++)
	{
		temp = a[i];
		a[i] = a[N - i - 1];
		a[N - i - 1] = temp;
	}
	printf("\nNow,array a:\n");
	for (i = 0; i < N; i++)
	{
		printf("%4d", a[i]);
	}
	printf("\n");
	return 0;
}

