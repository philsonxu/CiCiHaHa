#include <stdio.h>
#include <string.h>

int main()
{
	char buf1[1024], buf2[1024];
	printf("Please enter a string: ");
	scanf_s("%s", buf1, 1024);
	int m;
	printf("Please enter a location to start copying: ");
	scanf_s("%d", &m);
	if (m < 0 || m > strlen(buf1)) {//检测输入的位置是否合法
		printf("Illegal location entered\n");
		return -1;
	}
	char* ptr1 = buf1 + m; // 从第m个位置开始复制新数据
	char* ptr2 = buf2;
	while (*ptr1 != '\0') {
		*ptr2++ = *ptr1++;
	}
	*ptr2 = '\0';//不要忘了字符串结尾标志
	printf("%s\n", buf2);
	system("pause");
	return 0;
}


