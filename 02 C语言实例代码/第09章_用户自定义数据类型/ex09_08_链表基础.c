/*
 * 例9.8 链表基础（简单静态链表）
 * 知识点：结构体自引用（包含指向自身类型的指针）
 * 动态链表会用到malloc，是数据结构基础
 */
#include <stdio.h>

struct Node {
    int data;            /* 数据域 */
    struct Node *next;   /* 指针域：指向下一个节点 */
};

int main()
{
    /* 创建三个节点（静态方式）*/
    struct Node n1, n2, n3;
    struct Node *head, *p;
    
    n1.data = 10;
    n2.data = 20;
    n3.data = 30;
    
    /* 建立链接关系 */
    head = &n1;
    n1.next = &n2;
    n2.next = &n3;
    n3.next = NULL;  /* 尾节点指针为空 */
    
    /* 遍历链表 */
    printf("链表元素：");
    p = head;
    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("NULL\n");
    
    return 0;
}