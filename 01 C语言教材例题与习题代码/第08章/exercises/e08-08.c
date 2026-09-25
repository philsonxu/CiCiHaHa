#include <stdio.h>
#include <string.h>

int main()
{
	char buf[1024];
	printf("Please enter a string: ");
	gets_s(buf, 1024);
	int upper_count = 0, lower_count = 0, digit_count = 0, space_count = 0, other_count = 0;
	char* ptr = buf;
	while (*ptr != '\0') {
		if (*ptr >= 'A' && *ptr <= 'Z') { //大写字母
			upper_count++;
		}
		else if (*ptr >= 'a' && *ptr <= 'z') {//小写字母
			lower_count++;
		}
		else if (*ptr >= '0' && *ptr <= '9') {//数字字符
			digit_count++;
		}
		else if (*ptr == ' ') {//空格字符
			space_count++;
		}
		else { //其他字符
			other_count++;
		}
		ptr++;
	}
	printf("upper:%d; lower:%d; digit:%d; space:%d; other:%d\n", \
		upper_count, lower_count, digit_count, space_count, other_count);
	system("pause");
	return 0;
}


