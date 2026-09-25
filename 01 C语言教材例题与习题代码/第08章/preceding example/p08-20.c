#include <stdio.h>
int main()
{
	void cs(char from[], char to[]);
	char a[] = "I am a teacher.";
	char b[] = "You are a student.";
	char* from = a, * to = b;
	printf("string a=%s\nsring b=%s\n", a, b);
	printf("copy string a to string b:\n");
	cs(from, to);
	printf("string a=%s\nstring b=%s\n", a, b);
}
void cs(char from[], char to[])
{
	int i = 0;
	while (from[i] != '\0')
	{
		to[i] = from[i];
		i++;
	}
	to[i] = '\0';
}
