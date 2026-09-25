// 用选择法对 10个整数排序。

/*
解：选择排序的思路为∶设有10个元素 a[1]~a[10]，将 a[1]与 a[2~a[10]比较，若a[1]比a[2]~a[10]都小，则不进行交换，即无任何操作。
若 a[2]～a[10]中有一个以上比a[1]小，则将其中最大的一个（假设为a[i]与a[1]交换，此时 a[1]中存放了10个中最小的数。
第 2轮将 a[2]与 a[3]~a[10]比较，将剩下9个数中的最小者a[i]与a[2]对换，此时a[2]中存放的是10个中第二小的数。
依此类推，共进行9轮比较，a[1]~a[10]就已按由小到大的顺序存放了。
*/

#include<stdio.h>
int main()
{
	int i, j, min, temp, a[11];
	printf("enter data:\n");
	for (i = 1; i <= 10; i++)
	{
		printf("a[%d]=", i);
		scanf("%d", &a[i]);
	}
	printf("\n");
	printf("The orignal numbers:\n");
	for (i = 1; i <= 10; i++)
	{
		printf("%5d", a[i]);
	}
	printf("\n");
	for (i = 1; i <= 9; i++)
	{
		min = i;
		for (j = i + 1; j <= 10; j++)
		{
			if (a[min] > a[j])
			{
				min = j;
			}
		}
		temp = a[i];
		a[i] = a[min];
		a[min] = temp;
	}
	printf("\nThe sorted numbers:\n");
	for (i = 1; i <= 10; i++)
	{
		printf("%5d", a[i]);
	}
	printf("\n");
	return 0;
}

