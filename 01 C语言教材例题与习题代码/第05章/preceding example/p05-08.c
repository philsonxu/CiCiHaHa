// 求Fibonacci数列的前40个 数。这些数列有如下特点：第1，2两个数为1，1。
// 从第3个数开始，该数是前面两个数之和。
// F_1=1(n=1)
// F_2=1(n=2)
// F_n=F_{n-1} +F_{n-2}(n>=3)

#include<stdio.h>
int main()
{
	int f1=1,f2=1,f3;
	int i;
	printf("%12d\n%12d\n",f1,f2);
	for(i=1;i<=38;i++)
	{
		f3=f1+f2;
		printf("%12d\n",f3);
		f1=f2;
		f2=f3;
	}
	return 0;
}

