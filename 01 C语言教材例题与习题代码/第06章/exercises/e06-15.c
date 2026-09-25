/*
编写一个程序，将字符数组 s2 中的全部字符复制到字符数组 sl中。不用 strcpy 函数。
复制时，'\0'也要复制过去。'\0'后面的字符不复制。
*/

#include<stdio.h>
#include<string.h>
int main()
{
	char s1[80], s2[80];
	int i;
	printf("input s2:");
	scanf("%s", s2);
	for (i = 0; i <= strlen(s2); i++)
		s1[i] = s2[i];
	printf("s1:%s\n", s1);
	return 0;
}

