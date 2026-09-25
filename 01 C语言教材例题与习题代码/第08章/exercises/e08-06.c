#include <stdio.h>
int mystrlen(char* str)
{
	int len = 0;
	char* ptr = str;
	while (*ptr != '\0') {
		ptr++;
		len++;
	}
	return len;
}
int main()
{
	char buf[1024];
	printf("Please enter a string: ");
	scanf_s("%s", buf, 1024);
	printf("string len:%d\n", mystrlen(buf));
	//system("pause");
	return 0;
}

