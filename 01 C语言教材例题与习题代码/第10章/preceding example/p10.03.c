//从键盘读入若干字符串，对它们按字母大小的顺序排序，然后把排好序的字符串送到磁盘文件存。
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
	FILE* fp;
	char str[3][10], temp[10];
	int i, j, n = 3;
	printf("Enter string:\n");
	for (i = 0; i < n; i++)
		gets(str[i]);
	for (i = 0; i < n - 1; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (strcmp(str[i], str[j]) > 0)
			{
				strcpy(temp, str[i]);
				strcpy(str[i], str[j]);
				strcpy(str[j], temp);
			}
		}
	}
	if ((fp = fopen("file1.dat", "w")) == NULL)
	{
		printf("cant open file!!!\n");
		exit(0);
	}
	printf("\nThe new sequence:\n");
	for (i = 0; i < n; i++)
	{
		fputs(str[i], fp);
		fputs("\n", fp);
		printf("%s\n", str[i]);
	}
	return 0;
}

