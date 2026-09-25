#include <stdio.h>
int main()
{
	int a = 1, b = 2;
	printf("a=%d\tb=%d\n", a, b);
	int* p1, * p2;
	p1 = &a;
	p2 = &b;
	printf("*p1=%d\t*p2=%d", *p1, *p2);
}
