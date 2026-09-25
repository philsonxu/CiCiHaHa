#include<stdio.h>
#include<string.h>

int mystrcmp(char* str1, char* str2)
{
	char* ptr1 = str1;
	char* ptr2 = str2;
	int res;
	while (*ptr1 != '\0' && *ptr2 != '\0') {
		if (*ptr1 != *ptr2) {
			res = *ptr1 - *ptr2;
			break;
		}
		ptr1++;
		ptr2++;
	}
	if (*ptr1 == '\0' || *ptr2 == '\0') {//注意一个字符串到达结尾或者两个都到达结尾的情况
		res = *ptr1 - *ptr2;
	}
	return res;
}
int main()
{
	char buf1[1024] = { 0 };
	char buf2[1024] = { 0 };
	while (1) {
		printf("Please enter two strings:\n");
		gets_s(buf1, 1024);
		gets_s(buf2, 1024);
		printf("mystrcmp:%d", mystrcmp(buf1, buf2));
		printf("\n");
	}
	system("pause");
	return 0;
}


