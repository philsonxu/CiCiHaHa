#include<stdio.h>

int main()
{
	FILE* fp1, * fp2;
	char ch;
	fp1 = fopen("file1.dat", "r");

	fp2 = fopen("file2.dat", "w");
	ch = getc(fp1);
	while (!feof(fp1))
	{
		putchar(ch);
		ch = getc(fp1);
	}
	putchar(10);
	rewind(fp1);
	ch = getc(fp1);
	while (!feof(fp1))
	{
		fputc(ch, fp2);
		ch = fgetc(fp1);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}

