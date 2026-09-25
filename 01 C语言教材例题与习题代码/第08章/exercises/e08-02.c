#include <stdio.h>
int main()
{
	char str[3][32];
	char* p[3];
	printf("Please enter three strings:");
	for (int i = 0; i < 3; i++) {
		p[i] = str[i];
		scanf_s("%s", p[i], 32);//后边的数字限制缓冲区边界，防止缓冲区溢出访问越界
	}
	//让p[0]和p[1]/p[2]分别进行比较，找出最大的字符串，i+1之后，则让p[1]和p[2]进行比较，找出第二大
	//i循环总个数-1次,最后一个是不需要比较的
	for (int i = 0; i < 2; i++) {
		for (int j = i + 1; j < 3; j++) {
			if (strcmp(p[i], p[j]) > 0) {
				char* tmp = p[i]; p[i] = p[j]; p[j] = tmp;
			}
		}

	}
	printf("%s %s %s\n", p[0], p[1], p[2]);
	system("pause");
	return 0;
}

