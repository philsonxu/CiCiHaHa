#include <stdio.h>
int main()
{
	void s(int* p1, int* p2);
	int* p1, * p2, a, b;
	printf("please input two number:\n");
	scanf("%d %d", &a, &b);
	p1 = &a, p2 = &b;
	if (a < b)  s(p1, p2);
	printf("%d %d\n", a, b);
}
void s(int* p1, int* p2)
{
	int p;
	p = *p1, * p1 = *p2, * p2 = p;
}
