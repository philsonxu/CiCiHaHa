#include <stdio.h>
#include <stdlib.h>

typedef struct LNode
{
	int num;
	struct LNode* next;
} LNode;

//创建含有n个值的节点
LNode* creat(int n)
{
	LNode* head, * p;
	head = (LNode*)malloc(sizeof(LNode));
	p = head; //头节点为0 加上头节点共n + 1个节点
	head->num = 0;
	head->next = NULL;
	for (int i = 1; i <= n; i++)
	{
		LNode* newNode = (LNode*)malloc(sizeof(LNode));
		newNode->num = i;
		newNode->next = NULL;
		p->next = newNode;
		p = p->next;
	}
	return head;
}

//删除值为n的节点
void del(int n, LNode* head)
{
	LNode* pre, * current;
	pre = head;
	current = head->next;
	//从第一个有效节点开始查找待删除节点
	printf("delete node %d\n", n);
	while (current != NULL)
	{
		//找到待删除节点，重新链接，释放待删除节点
		if (current->num == n)
		{
			pre->next = current->next;
			free(current);
			break;
		}
		//更新查找位置
		pre = current;
		current = current->next;
	}
}

int main()
{
	LNode* head, * p;
	int n;
	head = creat(10);
	printf("请输入需要删除的节点:1-10\n");
	scanf("%d", &n);
	del(n, head);
	int i = 1;
	p = head->next;
	while (p != NULL)
	{
		printf("p %d.num -> %d\n", i, p->num);
		p = p->next;
		i++;
	}
	return 0;
}

