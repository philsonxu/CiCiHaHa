#include<stdio.h>
#include<math.h>

void reorder(int* arry, int n)
{
	int* start = arry;
	int* end = arry + n - 1;
	for (; start < end; start++, end--) {
		int tmp = *start;
		*start = *end;
		*end = tmp;
	}
	return;
}

int main()
{
	int arry[10];
	printf("Please enter ten numbers:");
	for (int i = 0; i < 10; i++) {
		scanf_s("%d", &arry[i]);
	}
	reorder(arry, 10);
	printf("\n");
	for (int i = 0; i < 10; i++) {
		printf("%d ", arry[i]);
	}
	printf("\n");
	system("pause");
	return 0;
}


