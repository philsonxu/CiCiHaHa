#include <stdio.h>

/*
* C 语言经典100例-01
* https://www.runoob.com/cprogramming/c-exercise-example1.html
* 题目：有 1、2、3、4 四个数字，能组成多少个互不相同且无重复数字的三位数？都是多少？
*/
int main()
{
	int i, j, k;
	printf("\n");
	// 以下为三重循环
	for (i = 1; i < 5; i++)
	{
		for (j = 1; j < 5; j++)
		{
			for (k = 1; k < 5; k++)
			{
				// 确保i、j、k三位互不相同
				if (i != k && i != j && j != k)
				{
					printf("%d,%d,%d\n", i, j, k);
				}
			}
		}
	}

	return 0;
}