#include <stdio.h>

void input(int* arry, int len)
{
	for (int i = 0; i < len; i++) {
		scanf_s("%d", &arry[i]);
	}
}
void print(int* arry, int len)
{
	for (int i = 0; i < len; i++) {
		printf("%d ", arry[i]);
	}
	printf("\n");
}
void handle(int* arry, int len)
{
	int max_num = arry[0], min_num = arry[0];
	int* p_max = NULL, * p_min = NULL;
	for (int i = 1; i < len; i++) {
		if (min_num > arry[i]) {//逐个比对后找出最小值
			min_num = arry[i];
			p_min = &arry[i];	//逐个比对后找到最小值的空间地址
		}
		if (max_num < arry[i]) {//逐个比对后找出最大值
			max_num = arry[i];
			p_max = &arry[i];   //逐个比对后找到最大值的空间地址
		}
	}
	int tmp;
	tmp = *p_min; *p_min = arry[0]; arry[0] = tmp;  //最小值与第一个数据交换
	tmp = *p_max; *p_max = arry[len - 1]; arry[len - 1] = tmp;//最大值与最后一个数据交换
}
int main()
{
	int arry[10];
	printf("Please enter ten nums:");
	input(arry, 10);
	handle(arry, 10);
	print(arry, 10);
	//system("pause");
	return 0;
}

