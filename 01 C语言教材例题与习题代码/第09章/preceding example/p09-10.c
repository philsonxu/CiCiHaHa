#include<stdio.h>
#include<malloc.h>
#define LEN sizeof(struct Student)
struct Student			//结构体 
{
	long num;
	float score;
	struct Student* next;
}*head, * p1, * p2;

void print(struct Student* head);
struct Student* creat();
void main()
{
	struct Student* head;
	head = creat();  //调用creat函数，返回第1个结点的起始地址 
	print(head);   //调用pritn函数 
}

//定义全局变量 
int n;			//作为指针指向下一个地址 

struct Student* creat()		//建立链表函数 
{
	p1 = p2 = (struct Student*)malloc(LEN);//开辟一个新单元 
	scanf("%ld %f", &p1->num, &p2->score);//输入第1个学生学号和成绩	

	while (p1->num != 0)
	{
		n += 1;
		if (n == 1)
			head = p1;
		else
			p2->next = p1;

		p2 = p1;		//p1的 
		p1 = (struct Student*)malloc(LEN);
		scanf("%ld %f", &p1->num, &p1->score);//输入第2个学生学号和成绩
	}
	p2->next = NULL;		//链表结束，赋值成链尾(NULL) 
	return (head);
}

void print(struct Student* head)		//输出链表函数
{
	struct Student* p;
	printf("\nNow,These %d records are:\n", n);
	p = head;			//head是主函数在建立链表里p1结构体指针的值分配到p的结构体指针进行输出 
	if (head != NULL)
		do
		{
			printf("%ld %5.1f\n", p->num, p->score);//输出打印内容 
			p = p->next;		//指向下个地址 
		} while (p != NULL);
}



