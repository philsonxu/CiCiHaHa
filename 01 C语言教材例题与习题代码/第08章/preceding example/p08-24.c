#include <stdio.h>
int main()
{
	int max(int, int);
	int min(int, int);
	int add(int, int);
	int(*p)(int, int);
	int a, b, c, n;
	printf("input a,b:");
	scanf("%d %d", &a, &b);
	printf("please input 1&2&3:");
	scanf("%d", &n);
	if (n == 1) p = max;
	else if (n == 2)p = min;
	else p = add;
	c = (*p)(a, b);
	printf("\na=%d\nb=%d\n", a, b);
	if (n == 1) printf("max=%d\n", c);
	else if (n == 2) printf("min=%d\n", c);
	else printf("add=%d\n", c);
}
int max(int x, int y)
{
	return(x > y ? x : y);
}
int min(int x, int y)
{
	return(x < y ? x : y);
}
int add(int x, int y)
{
	return(x + y);
}
