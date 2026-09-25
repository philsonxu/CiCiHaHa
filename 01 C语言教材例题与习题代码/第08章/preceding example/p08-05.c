#include <stdio.h>
int main()
{
	int* p1, * p2, * p3, p, a, b, c;
	printf("please input three numbers:\n");
	scanf("%d %d %d", &a, &b, &c);
	if (a < b) p = a, a = b, b = p;
	if (a < c) p = a, a = c, c = p;
	if (b < c) p = b, b = c; c = p;
	p1 = &a, p2 = &b, p3 = &c;
	printf("%d %d %d\n", a, b, c);
}
//三个数由大到小输出 
