#include <stdio.h>
int main()
{
	int a[5], i, * p;
	for (i = 0; i < 5; i++) scanf("%d", &a[i]);
	p = a;
	for (i = 0; i < 5; i++, p++) printf("%d", *p);
}
//输出数组 
