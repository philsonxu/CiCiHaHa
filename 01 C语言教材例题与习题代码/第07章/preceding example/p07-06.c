/* 有5个学生坐在一起，问第5个学生多少岁，他说比第4个学生大2岁。问第4个学生岁数，他说比第3个学生大2岁。
问第3个学生，又说比第2个学生大2岁。问第2个学生，说比第1个学生大2岁。最后问第1个学生，他说是10岁。
请问第5个学生多大。
*/

/*
解：年龄可用数学公式表示为
age(n)=10  (n=1)
age(n)=age(n-1)+2	(n>1)
*/

int age(int n)
{
	int c;
	if (n == 1)
		c = 10;
	else
		c = age(n - 1) + 2;
	return c;
}

#include<stdio.h>
int main()
{
	int age(int n);
	printf("NO.5,age:%d\n", age(5));
	return 0;
}

