// 找出一个二维数组中的鞍点，即该位置上的元素在该行上最大、在该列上最小。也可能没有鞍点。

/*
解：先找出一行中值最大的元素，然后检查它是否为该列中的最小值，如果是，则是鞍点（不需要再找别的鞍点了），
输出该鞍点，如果不是，再找下一行的最大数……如果每一行的最大数都不是鞍点，则此数组无鞍点。
*/

#include<stdio.h>
#define N 4
#define M 5		// 数组为4行5列

int main()
{
	int i, j, k, a[N][M], max, maxj, flag;
	printf("please input matrix:\n");
	for (i = 0; i < N; i++) 	// 输入数组
	{
		for (j = 0; j < M; j++)
		{
			scanf("%d", &a[i][j]);
		}
	}
	for (i = 0; i < N; i++)
	{
		max = a[i][0];	// 开始时假设a[i][0]最大 
		maxj = 0;			// 将列号0赋给maxj保存
		for (j = 0; j < M; j++) 	// 找出第i行中的最大数
		{
			if (a[i][j] > max)
			{
				max = a[i][j]; // 本行最大数存在max中 
				maxj = j;		// 将最大数所在的列数存在maxj中 
			}
		}
		flag = 1;
		for (k = 0; k < N; k++)
		{
			if (max > a[k][maxj]) // 将最大数和其同列元素相比
			{
				flag = 0;
				continue;
			}
		}
		if (flag)
		{
			printf("a[%d][%d]=%d\n", i, maxj, max);
			break;
		}
	}
	if (!flag)
	{
		printf("It is not exist!\n");
	}
	return 0;
}

/*
测试1
please input matrix:
1 2 3 4 5
2 4 6 8 10
3 6 9 12 15
4 8 12 16 20
a[0][4]=5
*/
/*
测试2
please input matrix:
1 2 3 4 11
2 4 6 8 12
3 6 9 19 15
4 8 12 16 7
It is not exist!
*/


