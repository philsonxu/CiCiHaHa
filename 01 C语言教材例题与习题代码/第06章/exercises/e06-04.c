// 有一个已排好序的数组，要求输入一个数后，按原来排序的规律将它插入数组中。

/*
解∶假设数组 a有n个元素，而且已按升序排列，在插入一个数时按下面的方法处理∶
（1）如果插人的数 num 比 a数组最后一个数大，则将插入的数放在 a数组末尾。
（2）如果插入的数 num不比 a 数组最后一个数大，则将它依次和a[0]～a[n-1]比较，
直到出现a[i]>num为止，这时表示 a[0]～a[i-1]各元素的值比 num小，a[i]～a[n-1]各元素的值比num大。


*/
#include<stdio.h>
int main()
{
	int a[11] = { 1,4,6,9,13,16,19,28,40,100 };
	int temp1, temp2, number, end, i, j;
	printf("array a:\n");
	for (i = 0; i < 10; i++)
	{
		printf("%5d", a[i]);
	}
	printf("\n");
	printf("insert data:");
	scanf("%d", &number);
	end = a[9];
	if (number > a[9])
	{
		a[10] = number;
	}
	else
	{
		for (i = 0; i < 10; i++)
		{
			if (a[i] > number)
			{
				temp1 = a[i];
				a[i] = number;
				for (j = i + 1; j < 11; j++)
				{
					temp2 = a[j];
					a[j] = temp1;
					temp1 = temp2;
				}
				break;
			}
		}
	}
	printf("New array a:\n");
	for (i = 0; i < 11; i++)
	{
		printf("%5d", a[i]);
	}
	printf("\n");
	return 0;
}

