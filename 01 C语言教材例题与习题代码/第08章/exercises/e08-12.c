#include<stdio.h>
#include<string.h>
#include<malloc.h>
void sort(char* s[10])
{
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = i; j < 10; j++) {
			if (strcmp(s[i], s[j]) > 0) {
				char* tmp = s[i]; //指针的好处在于直接通过赋值可以改变指向
				s[i] = s[j];  //只要交换了字符串的地址就实现了字符串的交换
				s[j] = tmp;//因此通过指针指向的交换就能够实现数组中字符串的排序
			}
		}
	}
}
int main()
{
	char* str[10];
	printf("Please enter ten strings:\n");
	for (int i = 0; i < 10; i++) {
		str[i] = malloc(32);//为每一个指针分配空间
		scanf_s("%s", str[i], 32);
	}
	sort(str);
	printf("\n");
	for (int i = 0; i < 10; i++) {
		printf("%s\n", str[i]);
		free(str[i]);
	}
	system("pause");
	return 0;
}


