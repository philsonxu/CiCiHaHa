#include <stdio.h>
int main()
{
	int i, a[5], * p;
	for (i = 4; i >= 0; i--) scanf("%d", &a[i]);
	p = a;
	for (i = 0; i < 5; i++, p++) printf("%d ", *p);
}
