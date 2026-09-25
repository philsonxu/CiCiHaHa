#include <stdio.h>
int main()
{
	int a[3][3], i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++) scanf("%d", &a[i][j]);
	}
	printf("%d,%d\n", a, *a);
	printf("%d,%d\n", a[0], *(a + 1));
	printf("%d,%d\n", a, &a[0][0]);
	printf("%d,%d\n", &a[1][0], a + 1);
	printf("%d,%d\n", &a[1][0], *(a + 1) + 0);
	printf("%d,%d\n", a[2], *(a + 2) + 0);
	printf("%d,%d\n", &a[2], a + 2);
	printf("%d,%d\n", a[1][0], *(*(a + 1) + 0));
	printf("%d,%d\n", a[2][0], *(*(a + 2)) + 0);
}
