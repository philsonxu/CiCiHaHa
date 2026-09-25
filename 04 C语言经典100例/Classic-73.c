#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include "classic.h"

/*
typedef struct LinkNode {
    int          data;
    struct LinkNode* next;
}LinkNode, * LinkList;

LinkList create_list(int n);
void print_list(LinkList h);
LinkList create_list(int n)
{
    LinkList L, p, q;
    int i;
    L = (LinkNode*)malloc(sizeof(LinkNode));
    if (!L)return 0;
    L->next = NULL;
    q = L;
    for (i = 1; i <= n; i++)
    {
        p = (LinkList)malloc(sizeof(LinkNode));
        printf("请输入第%d个元素的值:", i);
        scanf("%d", &(p->data));
        p->next = NULL;
        q->next = p;
        q = p;
    }
    return L;
}

void print_list(LinkList h)
{
    LinkList p = h->next;
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
}
*/
/*
* C 语言经典100例-73
* https://www.runoob.com/cprogramming/c-exercise-example73.html
* 题目：反向输出一个链表
*/
int main()
{
    LinkList Head = NULL;
    int n;

    printf("反向输出一个链表\n");
    printf("输入链表节点数：");
    scanf("%d", &n);
    Head = list_build(n);
    Head = list_reverse(Head);

    printf("刚刚建立的各个链表元素的值为：\n");
    list_print(Head);

    printf("\n\n");
    //system("pause");
    return 0;
}