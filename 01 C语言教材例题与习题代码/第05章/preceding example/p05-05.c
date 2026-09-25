// continue语句提前结束本次循环

// 要求输出100~200的不能被3整除的数

#include<stdio.h> 
int main()
{
	int n;
	for (n = 100; n <= 200; n++)
	{
		if (n % 3 == 0)
			continue;
		printf("%d ", n);
	}
	printf("\n");
	return 0;
}

