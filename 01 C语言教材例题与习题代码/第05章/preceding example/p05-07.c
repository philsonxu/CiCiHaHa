// 用公式pi/4≈1-1/3+1/5-1/7+...求pi得近似值，直至发现某一项的绝对值小于10^(-6)为止

#include<stdio.h> 
#include<math.h>
int main()
{
	int sign = 1;
	double pi = 0.0, n = 1.0, term = 1.0;// pi开始代表多项式的值，最后代表pi的值，n代表分母，term代表当前项的值

	while (fabs(term) >= 1e-6)
	{
		pi = pi + term;
		n = n + 2;
		sign = -sign;
		term = sign / n;
	}
	pi = pi * 4;
	printf("pi=%10.8f\n", pi);
	return 0;

}

