#include<stdio.h>
#include<math.h>
#define M 1000
/*
从键盘输入一个小于1000的正数，要求输出它的平方根（如平方根不是整数，则输出其整数部分）。
要求在输入数据后先对其进行检查是否为小于1000的正数。若不是，则要求重新输入。
*/
int main()
{
	int i, k;
	printf("请输入一个小于%d的整数i:", M);
	scanf("%d", &i);
	if (i > M)
	{
		printf("输入的数据不符合要求，请重新输入。");
		scanf("%d", &i);
	}
	k = sqrt(i);
	printf("%d的平方根的整数部分是%d\n", i, k);
}

