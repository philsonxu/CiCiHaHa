
#include <stdio.h>

/*
* C 语言经典100例-13
* https://www.w3cschool.cn/c/c-exercise-example13.html
* 题目：打印出所有的"水仙花数"
* 所谓"水仙花数"是指一个三位数，其各位数字立方和等于该数 本身。
* 例如：153 是一个"水仙花数"，因为 153=1 的三次方 ＋5 的三次方 ＋3 的三次方。
* 程序分析：利用 ​for​ 循环控制 100-999 个数，每个数分解出个位，十位，百位。
*/
int main()
{
	printf("100...1000之间的水仙花数\n");
	int i, x, y, z;
	for (i = 100; i < 1000; i++)
	{
		x = i % 10;
		y = i / 10 % 10;
		z = i / 100 % 10;
		if (i == (x * x * x + y * y * y + z * z * z))
		{
			printf("%d^3 + %d^3 + %d^3 = %d\n", z, y, x, i);
		}
	}

	return 0;
}