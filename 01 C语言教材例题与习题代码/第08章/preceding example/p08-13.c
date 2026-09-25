#include <stdio.h>
int main()
{
	int a[3][3] = { 1,2,3,4,5,6,7,8,9 };
	int (*p)[3], i, j;
	p = a;
	scanf("%d %d", &i, &j);
	printf("a[%d][%d]=%d\n", i, j, *(*(p + i) + j));
}
