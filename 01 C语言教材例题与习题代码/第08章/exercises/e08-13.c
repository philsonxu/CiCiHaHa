#include<stdio.h>
#include<math.h>

double integral(double(*handler)(double), double a, double b, int n)
{
	double i, s = 0;
	double h = (b - a) / n;
	for (i = a; i <= b; i += h) {
		s += handler(i) * h;
	}
	return s;
}
int main()
{
	double a, b;
	int n = 200000; // 区间划分个数
	int func_idx; // 计算函数选择id
	printf("Please enter the lower and upper limit of integration:");
	scanf_s("%lf %lf", &a, &b);
	printf("Please enter specific calculation function(1-sin/2-cos/3-exp): ");
	scanf_s("%d", &func_idx);
	switch (func_idx) {
	case 1:printf("The integral of sin(x) is:%lf\n", integral(sin, a, b, n)); break;
	case 2:printf("The integral of cos(x) is:%lf\n", integral(cos, a, b, n)); break;
	case 3:printf("The integral of exp(x) is:%lf\n", integral(exp, a, b, n)); break;
	default:
		printf("function id error\n");
		return -1;
	}
	system("pause");
	return 0;
}


