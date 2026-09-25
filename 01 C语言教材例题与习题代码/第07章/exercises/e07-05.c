#include <stdio.h>
#include <string.h>

void ReverseString(char str[])
{
	int start, end;
	char tmp;
	start = 0;
	end = strlen(str) - 1; //字符数组小标从0开始，所以-1
	while (start < end)
	{
		tmp = str[start];
		str[start] = str[end];
		str[end] = tmp;

		start++;
		end--;
	}
}

int main()
{
	char str[100] = { 0 };
	printf("请输入一个字符串:>");
	scanf("%s", str);
	printf("原始字符串为:> %s\n", str);
	ReverseString(str);
	printf("反序字符串为:> %s\n", str);
	return 0;
}

