#include<stdio.h>
#include<string.h>

void count_digit(char* str)
{
	int digit[32], count = 0;
	char* ptr = str;
	int i = 0, str_len = strlen(str);
	while (i < str_len) {
		if (*(ptr + i) >= '0' && *(ptr + i) <= '9') {
			int len = 1;//用于统计连续数字的个数
			while (*(ptr + i + len) >= '0' && *(ptr + i + len) <= '9' && (i + len) < str_len) {//找出从当前位置连续数字的个数
				len++;
			}
			int sum = *(ptr + i + len - 1) - '0';//先获取个位数的数据
			int unit = 1;//每一位的单位，从十位开始每次乘以10作为单位
			for (int j = len - 2; j >= 0; j--) {//从右往左逐个处理
				unit *= 10;
				sum += (*(ptr + i + j) - '0') * unit;
			}
			digit[count++] = sum;
			i += len; // i需要加上len的长度，越过这个数字，防止一个连续数字中的字符重复处理
			continue;
		}
		i++;
	}
	for (int i = 0; i < count; i++) {
		printf("%d ", digit[i]);
	}
	return;
}
int main()
{
	char buf[1024] = { 0 };
	printf("Please enter a string with numbers:\n");
	gets_s(buf, 1024);
	count_digit(buf);
	printf("\n");
	system("pause");
	return 0;
}


