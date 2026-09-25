#include<stdio.h>

double polya(int n, int x)
{
	double result;
	if (n == 0)
		result = 1;
	if (n == 1)
		result = x;
	if (n > 1)
		result = ((2 * n - 1) * x * polya(n - 1, x) - (n - 1) * polya(n - 2, x)) / n;
	return result;
}

int main()
{
	int x, n;
	scanf("%d %d", &n, &x);
	printf("%.2f\n", polya(n, x));
	return 0;
}

