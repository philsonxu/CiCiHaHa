#include<stdio.h>
#include<math.h>

float avg(int arry[][5], int n)
{
	float sum = 0;
	for (int i = 0; i < n; i++) {
		sum += arry[i][0];
	}
	printf("Average of course 1:%f\n", (sum / n));
	return (sum / n);
}
void fail(int arry[][5], int n)
{
	printf("Information on students who fail in more than two courses: ");
	for (int i = 0; i < n; i++) {
		int sum = 0, fail_count = 0;
		for (int j = 0; j < 5; j++) {
			if (arry[i][j] < 60) {
				fail_count++;
			}
		}
		if (fail_count <= 2) {
			continue;
		}
		printf("seq:%d ", i + 1);
		printf("score: ");
		for (int j = 0; j < 5; j++) {
			sum += arry[i][j];
			printf("%d ", arry[i][j]);
		}
		printf("avg:%d ", sum / 5);
		printf("\n");
	}
	return;
}
void excellent(int arry[][5], int n)
{
	int i, j;
	for (i = 0; i < n; i++) {
		int sum = 0, count = 0;
		for (j = 0; j < 5; j++) {
			sum += arry[i][j]; //计算总分用于计算平均分
			if (arry[i][j] > 85) {
				count++; //若每门课都大于85则count总会与j同步++
			}
		}
		if ((sum / 5) > 90 || count == j) {
			printf("Excellent students: %d\n", i + 1);
		}
	}
	return;
}

int main()
{
	int arry[4][5];
	printf("Please enter a 4x5 matrix:\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			scanf_s("%d", &arry[i][j]);
		}
	}
	avg(arry, 4);
	fail(arry, 4);
	excellent(arry, 4);
	printf("\n");
	system("pause");
	return 0;
}


