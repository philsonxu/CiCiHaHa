#include <stdio.h>//按数的大小顺序输出 
int main()
{
	int a[5], i, j, n, * p;
	for (i = 0; i < 5; i++) scanf("%d", &a[i]);
	for (i = 0; i < 4; i++)
	{
		for (j = i + 1; j < 5; j++)
		{
			if (a[i] > a[j])
			{
				n = a[i]; a[i] = a[j]; a[j] = n;
			}
		}
	}
	p = a;
	for (i = 0; i < 5; i++, p++) printf("%d ", *p);
	printf("\n");
}
