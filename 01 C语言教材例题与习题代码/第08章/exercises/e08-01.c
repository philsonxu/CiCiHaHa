#include <stdio.h>

void swap(int* p_a, int* p_b)
{
	int temp = *p_a;
	*p_a = *p_b;
	*p_b = temp;
}
int main()
{
	int a, b, c, * p_a = &a, * p_b = &b, * p_c = &c;	 // 获取每个变量空间的地址 
	printf("Please enter three numbers:");
	scanf_s("%d%d%d", p_a, p_b, p_c);
	if (*p_a > *p_b) {
		swap(p_a, p_b);//通过指针进行指向空间内的数据交换
	}
	if (*p_a > *p_c) {
		swap(p_a, p_c);
	}
	if (*p_b > *p_c) {
		swap(p_b, p_c);
	}
	printf("%d %d %d\n", *p_a, *p_b, *p_c);
	system("pause");
	return 0;
}

