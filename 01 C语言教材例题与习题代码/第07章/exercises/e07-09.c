#include<stdio.h>

int letter, digit, space, others;

void CountChar(char str[])
{
	int i;
	for (i = 0; str[i] != '\0'; i++)
	{
		//统计字母
		if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
			letter++;
		else if (str[i] >= '0' && str[i] <= '9') //统计数字
			digit++;
		else if (str[i] == ' ')//统计空格
			space++;
		else
			others++;  //统计其他字符
	}
}

int main()
{
	char text[80];
	printf("input string:\n");
	gets(text);
	printf("string: %s\n", text);

	CountChar(text);
	printf("\nletter:%d\ndigit:%d\nspace:%d\nothers:%d\n", letter, digit, space, others);
	return 0;
}

