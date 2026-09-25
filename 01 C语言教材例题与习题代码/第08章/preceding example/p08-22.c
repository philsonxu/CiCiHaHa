#include <stdio.h>
int main()
{
	int max(int, int);
	int (*p)(int, int);
	int a, b, c;
	printf("input a,b:");
	scanf("%d %d", &a, &b);
	p = max;
	printf("a=%d,b=%d\n", a, b);
	c = (*p)(a, b);
	printf("max=%d", c);
}
int max(int x, int y)
{
	return (x > y ? x : y);
}
