/*
一个数如果恰好等于它的因子之和，这个数就称为"完数"。
例如，6的因子为1，2，3，而6=1十2十3，因此6是"完数"。
编程序找出1000之内的所有完数，并按下面格式输出其因子∶
6 its factors are 1 2 3
*/

#include<stdio.h> 
int main()
{
	int s, i, m;
	for (m = 2; m < 1000; m++)
	{
		s = 0;
		for (i = 1; i < m; i++)
		{
			if ((m % i) == 0)
				s = s + i;
		}
		if (s == m)
		{
			printf("%d,its factors are ", m);
			for (i = 1; i < m; i++)
				if (m % i == 0)
					printf("%d ", i);
			printf("\n");
		}
	}
	return 0;
}

