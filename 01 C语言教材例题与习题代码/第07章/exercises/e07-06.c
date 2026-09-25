#include<stdio.h>

void ConcatStr(char string1[], char string2[], char string[])
{
	int i, j;
	for (i = 0; string1[i] != '\0'; i++)
		string[i] = string1[i];

	//找到字符串末尾，继续往后面链接字符串
	for (j = 0; string2[j] != '\0'; j++)
		string[i + j] = string2[j];

	//字符串末尾加上结束符 \0
	string[i + j] = '\0';
}

int main()
{
	char s1[200] = { 0 }, s2[100] = { 0 }, s[100] = { 0 };
	printf("input string1:");
	scanf("%s", s1);
	printf("input string2:");
	scanf("%s", s2);
	ConcatStr(s1, s2, s);
	printf("\nThe new string is %s\n", s);
	return 0;
}

