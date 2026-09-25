#include<stdio.h>

size_t HextoDec(char s[])
{
	size_t i, n;
	n = 0;
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
			n = n * 16 + s[i] - '0';
		if (s[i] >= 'a' && s[i] <= 'f')
			n = n * 16 + s[i] - 'a' + 10;
		if (s[i] >= 'A' && s[i] <= 'F')
			n = n * 16 + s[i] - 'A' + 10;
	}
	return n;
}

int main()
{
	size_t result = 0;
	char hex[9] = { 0 };
	printf("input a HEX number:");
	scanf("%s", hex);

	result = HextoDec(hex);
	printf("0x%s = %u\n", hex, result);

	return 0;
}

