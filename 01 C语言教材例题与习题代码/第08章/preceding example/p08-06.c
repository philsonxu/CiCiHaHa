#include <stdio.h>
int main()
{
	int a[3];
	int i;
	printf("input 10 numbers:\n");
	for (i = 0; i < 3; i++) scanf("%d", &a[i]);
	for (i = 0; i < 3; i++) printf("%d ", *(a + i));
}
//输出数组 
