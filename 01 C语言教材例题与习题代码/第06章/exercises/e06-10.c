// 有一篇文章，共有3行文字，每行有 80个字符。要求分别统计出其中英文大写母、小写字母、数字、空格以及其他字符的个数。 

#include <stdio.h> 
#include <conio.h>
#include <stdlib.h>

int main()
{
	int i, j, upp, low, dig, spa, oth;
	char text[3][80];
	upp = low = dig = spa = oth = 0;
	for (i = 0; i < 3; i++)
	{
		printf("please input line %d:\n", i + 1);
		gets(text[i]);
		for (j = 0; j < 80 && text[i][j] != '\0'; j++)
		{
			if (text[i][j] >= 'A' && text[i][j] <= 'Z')
				upp++;
			else if (text[i][j] >= 'a' && text[i][j] <= 'z')
				low++;
			else if (text[i][j] >= '0' && text[i][j] <= '9')
				dig++;
			else if (text[i][j] == ' ')
				spa++;
			else
				oth++;
		}
	}
	printf("\nupper case: %d\n", upp);
	printf("lower case: %d\n", low);
	printf("digit     : %d\n", dig);
	printf("space     : %d\n", spa);
	printf("other     : %d\n", oth);
	return 0;
}

