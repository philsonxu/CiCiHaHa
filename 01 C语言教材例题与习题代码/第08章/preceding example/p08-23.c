#include <stdio.h>
int main()
{
	int max(int x, int y);
	int min(int x, int y);
	int (*p)(int, int);
	int a, b, c, n;
	printf("input number:");
	scanf("%d %d", &a, &b);
	printf("choose 1&2:\n");
	scanf("%d", &n);
	printf("a=%d,b=%d\n", a, b);

	if (n == 1)
	{
		p = max;
	}
	else
	{
		p = min;
	}
	c = (*p)(a, b);
	printf("%d\n", c);
}
int max(int x, int y)
{
	return (x > y ? x : y);
}
int min(int x, int y)
{
	return(x < y ? x : y);
}
