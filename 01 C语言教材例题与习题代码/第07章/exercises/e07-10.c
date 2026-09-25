#include<stdio.h>
#include<string.h>

void LongestWord(char str[], char word[])
{
	int max_len = 0;
	int len = 0;
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ' ')
		{
			str[i] = '\0';
			len = strlen(str);
			if (len > max_len)
			{
				max_len = len;
				strcpy(word, str);
				str = str + len + 1;
			}
		}
		i++;
	}
}

int main()
{
	char line[100] = { 0 };
	char word[100] = { 0 };
	printf("input one line:\n");
	gets(line);

	LongestWord(line, word);

	printf("The longest word is : %s\n", word);
	return 0;
}

