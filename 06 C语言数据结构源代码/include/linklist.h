/**
 * 单链表（带头结点）—— 线性表的链式存储实现
 * 特性：插入删除O(1)（已知前驱），随机访问O(n)，无需预分配空间
 */
#ifndef LINKLIST_H
#define LINKLIST_H

#include "common.h"

typedef struct LNode {
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

/* 初始化带头结点的空链表 */
Status LinkList_Init(LinkList *L);
/* 销毁链表 */
Status LinkList_Destroy(LinkList *L);
/* 清空链表（保留头结点） */
Status LinkList_Clear(LinkList L);
/* 判空 */
bool   LinkList_Empty(LinkList L);
/* 长度 */
int    LinkList_Length(LinkList L);
/* 按位取值 */
Status LinkList_Get(LinkList L, int i, ElemType *e);
/* 按值查找（返回节点指针） */
LNode* LinkList_Locate(LinkList L, ElemType e, CompareFunc cmp);
/* 在第i个位置插入 */
Status LinkList_Insert(LinkList L, int i, ElemType e);
/* 删除第i个位置 */
Status LinkList_Delete(LinkList L, int i, ElemType *e);
/* 头插法建表（逆序输入） */
Status LinkList_Create_Head(LinkList *L, ElemType arr[], int n);
/* 尾插法建表（顺序输入） */
Status LinkList_Create_Tail(LinkList *L, ElemType arr[], int n);
/* 遍历 */
Status LinkList_Traverse(LinkList L, VisitFunc visit);

/* 链表反转（原地反转） */
Status LinkList_Reverse(LinkList L);
/* 查找倒数第k个节点（快慢指针一次遍历） */
LNode* LinkList_FindLastK(LinkList L, int k);
/* 检测环并求入环节点（Floyd判圈算法） */
LNode* LinkList_DetectCycle(LinkList L);

#endif /* LINKLIST_H */
