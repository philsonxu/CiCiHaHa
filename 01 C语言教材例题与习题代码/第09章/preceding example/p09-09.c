//写一个函数建立一个有3名学生数据的单向动态链表
#include <stdio.h>
#include <stdlib.h>
#define LEN sizeof(struct Student)
struct Student {
	long num;
	float score;
	struct Student* next;
};
//定义全局变量
int n;
struct Student* creat(void) {
	struct Student* head;
	struct Student* p1, * p2;
	n = 0;
	p1 = p2 = (struct Student*)malloc(LEN);
	scanf_s("%ld%f", &p1->num, &p1->score);
	head = NULL;
	while (p1->num != 0) {
		n = n + 1;
		if (n == 1) {
			head = p1;
		}
		else {
			p2->next = p1;
			p2 = p1;
			p1 = (struct Student*)malloc(LEN);
			scanf_s("%ld%f", &p1->num, &p1->score);
		}
	}
	p2->next = NULL;
	return head;
}

int main() {
	struct Student* pt;
	pt = creat();
	printf("学号为：%ld\n成绩为：%f\n", pt->num, pt->score);
	return 0;
}

