#include<stdio.h>
#include<string.h>

void BubbleSort(char str[])
{
	int i, j;
	char tmp;
	int len = strlen(str);
	for (i = 0; i < len - 1; ++i)
	{
		for (j = 0; j < len - i - 1; ++j)
		{
			if (str[j] > str[j + 1])
			{
				tmp = str[j];
				str[j] = str[j + 1];
				str[j + 1] = tmp;
			}
		}
	}
}

int main()
{
	int i;
	char str[11] = { 0 };
	printf("请输入10个字符:>");
	for (i = 0; i < 10; ++i)
		scanf("%c", &str[i]);

	BubbleSort(str);

	printf("string sorted: %s\n", str);
	return 0;
}

