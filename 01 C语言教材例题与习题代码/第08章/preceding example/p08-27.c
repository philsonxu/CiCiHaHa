#include <stdio.h>
#include <string.h> 
int main()
{
	void sort(char* name[], int n);
	void print(char* name[], int n);
	char* name[5] = { "Follow me","Great Wall","Happy","EXCUSE","Thanks" };
	int n = 5;
	sort(name, n);
	print(name, n);
}
void sort(char* name[], int n)
{
	char* temp;
	int i, k, j;
	for (i = 0; i < n - 1; i++)
	{
		k = i;
		for (j = i + 1; j < n; j++)
			if (strcmp(name[k], name[i]) > 0) k = j;
		if (k != i)
		{
			temp = name[i]; name[i] = name[k]; name[k] = temp;
		}
	}
}
void print(char* name[], int n)
{
	int i;
	for (i = 0; i < 5; i++) printf("%s\n", name[i]);
}
