#include <stdio.h>
int main()
{
	void inv(int* x, int n);
	int i, a[5], * p = a;
	printf("original array:\n");
	for (i = 0; i < 5; i++, p++) scanf("%d", p);
	p = a;
	inv(p, 5);
	printf("inverted array:\n");
	for (p = a; p < a + 5; p++) printf("%d ", *p);
}
void inv(int* x, int n)
{
	int* p, m, temp, * i, * j;
	m = (n - 1) / 2;
	i = x; j = x + n - 1; p = x + m;
	for (; i <= p; i++, j--) temp = *i, * i = *j, * j = temp;
}
