#include<stdio.h>
#include<string.h>
void sort(char s[10][32])
{
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = i; j < 10; j++) {
			if (strcmp(s[i], s[j]) > 0) {
				char tmp[32];
				strcpy_s(tmp, 32, s[i]);
				strcpy_s(s[i], 32, s[j]);
				strcpy_s(s[j], 32, tmp);
			}
		}
	}
}
int main()
{
	char str[10][32];
	printf("Please enter ten strings:\n");
	for (int i = 0; i < 10; i++) {
		scanf_s("%s", str[i], 32);
	}
	sort(str);
	printf("\n");
	for (int i = 0; i < 10; i++) {
		printf("%s\n", str[i]);
	}
	system("pause");
	return 0;
}


