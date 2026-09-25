// 输入10个数，要求输出其中值最大的元素和该数是第几个数。

#include<stdio.h>
int main()
{
	int max(int x, int y);
	int a[10], m, n, i;
	printf("enter 10 integer numbers:");
	for (i = 0; i < 10; i++)
		scanf("%d", &a[i]);
	printf("\n");
	for (i = 1, m = a[0]; i < 10; i++)
	{
		if (max(m, a[i]) > m)
		{
			m = max(m, a[i]);
			n = i;
		}
	}
	printf("The largest number is %d\nit is the %dth number.\n", m, n + 1);
}

int max(int x, int y)
{
	return (x > y ? x : y);
}

