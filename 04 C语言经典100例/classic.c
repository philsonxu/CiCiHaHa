#include <stdio.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include "classic.h"

/*
* 判断 year 是不是闰年的函数
*/
int is_leap_year(int year)
{
	// 闰年的判断规则：能被4整除且（不能被100整除或能被400整除）
	if ((year % 4 == 0 && year % 100 != 0) ||
		(year % 400 == 0))
	{
		return 1;
	}
	return 0;
}

/*
* 交换两个int变量的值
*/
void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

/*
* 交换两个字符串
*/
void strswap(char* str1, char* str2) {
	char temp[256];
	strcpy(temp, str1); // 将 str1 复制到临时字符串 temp
	strcpy(str1, str2); // 将 str2 复制到 str1
	strcpy(str2, temp); // 将 temp 复制到 str2
}

/*
* 阶乘
*/
unsigned long long factorial(int n)
{
	unsigned long long result = 1;
	for (int i = 1; i <= n; i++) {
		result *= i;
	}
	return result;
}

/*
* 递归法计算阶乘
*/
unsigned long long factorial_recursion(int n)
{
	// 定义变量存储结果
	unsigned long long result;

	// 阶乘的递归基准条件
	if (n == 0) {
		result = 1; // 0! = 1
	}
	else {
		result = n * factorial_recursion(n - 1); // 递归计算 n! = n * (n-1)!
	}
	return result; // 返回计算结果
}

/*
* 字符串反转
*/
void strrevs(char* s)
{
	// 获取字符串长度
	int len = 0;
	char* p = s;
	while (*p != 0)
	{
		len++;
		p++;
	}

	// 交换 ...
	int i = 0;
	char c;
	while (i <= len / 2 - 1)
	{
		c = *(s + i);
		*(s + i) = *(s + len - 1 - i);
		*(s + len - 1 - i) = c;
		i++;
	}
}

/*
* 按二进制01打印数字
*/
void print_binary(int num) {
	int bits = sizeof(int) * CHAR_BIT;
	for (int i = bits - 1; i >= 0; i--) {
		putchar((num >> i) & 1 ? '1' : '0');
		if (i % 4 == 0 && i != 0) putchar(' ');
	}
	printf(" %12u ", num);
	putchar('\n');
}

/*
* 按二进制01打印两个数字的竖式运算
*/
void print_binary_formula(int a, int b, int c)
{
	print_binary(a);
	print_binary(b);
	printf("------------------------------------------\n");
	print_binary(c);
	printf("\n");
}

/*
* 创建链表
*/
LinkList list_build(int n)
{
	LinkList head, p, q;
	int i = 1;
	head = (LinkNode*)malloc(sizeof(LinkNode));
	if (!head)return 0;
	printf("请输入第%d个元素的值:", i);
	scanf("%d", &(head->data));
	head->next = NULL;
	q = head;
	for (i = 1; i < n; i++)
	{
		p = (LinkList)malloc(sizeof(LinkNode));
		printf("请输入第%d个元素的值:", i+2);
		scanf("%d", &(p->data));
		p->next = NULL;
		q->next = p;
		q = p;
	}
	return head;
}

/*
* 链表反转
*/
LinkList list_reverse(LinkList head)
{
	LinkList prep, p, q;
	p = head->next;
	q = p->next;
	p->next = NULL;
	while (q)
	{
		prep = p;
		p = q;
		q = q->next;
		p->next = prep;
	}
	head->next = p;
	return head;
}

/*
* 链表打印
*/
void list_print(LinkList head)
{
	LinkList p = head;
	while (p != NULL) {
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
}

/*
* 链表节点删除
*/
LinkList list_delete_node(LinkList head, LinkList tmp)
{
	if (tmp == NULL)
		return head->next;
	else
	{
		if (tmp->next->next == NULL)
			tmp->next = NULL;
		else
			tmp->next = tmp->next->next;
		return head;
	}
}

/*
* 链表排序
*/
/*
void list_sort(LinkList head, int num)
{
	LinkList tmp, btmp;
	int i, min;
	for (i = 0; i < num; i++)
	{
		tmp = head;
		min = tmp->data;
		btmp = NULL;
		while (tmp->next)
		{
			if (min > tmp->next->data)
			{
				min = tmp->next->data;
				btmp = tmp;
			}
			tmp = tmp->next;
		}
		head = list_delete_node(head, btmp);
	}
}
*/
void list_sort(LinkList head, int num)
{
	LinkNode* p, * q, * m, * n;
	LinkNode* temp1, * temp2;
	if (head->next == NULL)
	{
		return;
	}
	else
	{
		p = head; q = head->next;
		while (q->next != NULL)
		{
			m = p->next;
			n = q->next;
			temp1 = m;
			while (temp1->next != NULL)
			{
				if (temp1->next->data < q->data && temp1->next->data < n->data)
				{
					m = temp1; n = temp1->next;
				}
				temp1 = temp1->next;
			}
			if (m != p->next || (m == p->next && m->data > n->data))
			{
				p->next = n;
				p = n;
				m->next = q;
				m = q;
				q = q->next;
				n = n->next;
				p->next = q;
				m->next = n;
			}
			else
			{
				p = p->next;
				q = q->next;
			}
		}
		temp2 = head->next;
	}
}

/*
* 从数组创建链表
*/
LinkList list_create_array(int array[], int num)
{
	LinkList head, p, q;
	int i;
	head = (LinkNode*)malloc(sizeof(LinkNode));
	if (!head)return 0;
	head->data = array[0];
	head->next = NULL;
	q = head;
	for (i = 1; i < num; i++)
	{
		p = (LinkList)malloc(sizeof(LinkNode));
		p->data = array[i];
		p->next = NULL;
		q->next = p;
		q = p;
	}
	return head;
}

/*
* 链表合并
*/
LinkList list_concatenate(LinkList head1, LinkList head2)
{
	LinkList tmp;
	tmp = head1;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = head2;
	return head1;
}
