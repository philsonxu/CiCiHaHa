/*
编一个程序，将两个字符串sl和 s2 比较，若s1>s2，输出一个正数;若sl=s2，输出0;若s1<s2，输出一个负数。
不要用strcpy 函数。两个字符串用gets 函数读入。输
出的正数或负数的绝对值应是相比较的两个字符串相应字符的 ASCII码的差值。
例如，"A"与"C"相比，由于"A"<"C"，应输出负数，同时由于'A'与'C'的 ASCII码差值为 2，因此应输出-2。同
理∶"And"和"Aid"比较，根据第 2个字符比较结果，'n'比'i'大5，因此应输出5。
*/

#include<stdio.h> 
int main()
{
	int i, resu;
	char s1[100], s2[100];
	printf("input string1:");
	gets(s1);
	printf("\ninput string2:");
	gets(s2);
	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != '\0'))
		i++;
	if (s1[i] == '\0' && s2[i] == '\0')
		resu = 0;
	else
		resu = s1[i] - s2[i];
	printf("\nresult:%d.\n", resu);
	return 0;
}

