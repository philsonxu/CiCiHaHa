#include <stdio.h>
void move(int* arry, int n, int m)
{
	int end_idx = n - m;//找到倒数第m个数据的位置，也就是要移动到数组首部的数据的起始位置
	for (int i = 0; i < m; i++) {
		int* p = arry + end_idx + i;//从倒数第m个数据的位置开始逐渐向后偏移直到数组尾部
		int tmp = *p;//获取到这个位置的值，用于数组前边数据向后偏移1位之后，向数组第i位赋值
		for (int j = end_idx + i; j > i; j--) {//从第i位开始逐个向后偏移一位
			*p = *(p - 1);
			p--;
		}
		*(arry + i) = tmp;//数组的前第i个数字逐个替换为后边被覆盖的值
	}
}
int main()
{
	int number[32], n, m, i;
	printf("Please enter the number of numbers: ");
	scanf_s("%d", &n);//先确定要输入多少个数字
	printf("Please enter %d numbers: ", n);
	for (i = 0; i < n; i++) {
		scanf_s("%d", &number[i]);//输入指定个数的数字
	}
	printf("Number of positions to move: ");
	scanf_s("%d", &m);//确定要向后移动多少个位置
	move(number, n, m);
	for (i = 0; i < n; i++) {
		printf("%d ", number[i]);
	}
	printf("\n");
	system("pause");
	return 0;
}

