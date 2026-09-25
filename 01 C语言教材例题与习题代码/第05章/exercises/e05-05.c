/*
求S，=a+aa+aaa+…+aa…a(n个a)之值，其中a是一个数字，n表示a的位数，n由键盘输入，
例如∶2＋22＋222＋2222＋22222（此时n=5）。
*/

#include<stdio.h>
int main()
{
	int a, n, i = 1, sn = 0, tn = 0;
	printf("a,n=:");
	scanf("%d,%d", &a, &n);
	while (i <= n)
	{
		tn = tn + a; // 赋值后的tn为i个a组成数的值
		sn = sn + tn;// 赋值后的sn为多项式前i想之和 
		a = a * 10;
		++i;
	}
	printf("a+aa+aaa+...=%d\n", sn);
	return 0;
}

