#include <stdio.h>
typedef struct student
{
	int num;
	double grade;
	struct student* next;
} student;
student* del(student* a, student* b)
{
	student* pre, * current, * head;
	head = a;

	while (b != NULL)
	{
		//重置指针指向a链表的头部
		pre = head;
		current = head->next;
		//a 链表的头等于b
		if (pre->num == b->num)
		{
			pre->next = NULL;
			pre = current;
			current = current->next;
			//更新表头
			head = pre;
		}
		else
		{
			while (pre->next != NULL)
			{
				if (current->num == b->num)
				{
					//找到就删除
					pre->next = current->next;
					break;
				}
				else
				{
					//否则继续遍历
					pre = pre->next;
					current = current->next;
				}
			}
		}
		b = b->next;
	}
	return head;
}

void printList(student* root)
{
	printf("----\n");
	int i = 0;
	while (root != NULL)
	{
		printf("student %d -> %d -> %.2lf\n", i, root->num, root->grade);
		root = root->next;
		i++;
	}
}

int main()
{
	student a[3] = { { 1, 79 }, { 4, 36 }, { 5, 79 } };
	for (int i = 0; i < 2; i++)
	{
		a[i].next = &a[i + 1];
	}
	a[2].next = NULL;
	printf("链表a:\n");
	printList(&a[0]);

	student b[2] = { { 5, 38 }, { 4, 98 } };
	for (int i = 0; i < 1; i++)
	{
		b[i].next = &b[i + 1];
	}
	b[1].next = NULL;
	printf("链表b:\n");
	printList(&b[0]);
	student* combine = del(a, b);
	printf("删除之后：\n");
	while (combine != NULL)
	{
		printf("%d -> %.2lf\n", combine->num, combine->grade);
		combine = combine->next;
	}

	return 0;
}

