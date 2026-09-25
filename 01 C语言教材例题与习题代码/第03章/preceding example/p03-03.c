// 给定一个大写字母，要求用小写字母输出
// 小写字母ASCII码值比大写字母大32

#include<stdio.h> 
int main()
{
	char c1, c2;
	c1 = 'A';
	c2 = c1 + 32;
	printf("%c\n", c2);
	printf("%d\n", c2);
	return 0;
}

