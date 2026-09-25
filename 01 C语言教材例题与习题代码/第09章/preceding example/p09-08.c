//建立静态链表，有3个学生，信息包括学号和成绩，输出这些信息

#include<stdio.h>


int main()
{
	struct student
	{
		int number;
		float score;
		struct sutdent* next;  //只有指向下一个结点的本结构体类型的结点才能形成链表（胡言乱语，懂自懂
	};
	struct student a, b, c, * head, * p;
	a.number = 11;
	a.score = 98;
	b.number = 12;
	b.score = 96;
	c.number = 13;
	c.score = 99;
	head = &a;
	a.next = &b;
	b.next = &c;
	c.next = NULL;
	p = head;
	do
	{
		printf("%2d%7.2f\n", p->number, p->score);
		p = p->next;
	} while (p != NULL);


	return 0;
}

