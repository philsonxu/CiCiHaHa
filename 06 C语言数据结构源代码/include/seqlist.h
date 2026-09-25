/**
 * 顺序表（动态数组）—— 线性表的顺序存储实现
 * 特性：随机访问O(1)，插入删除平均O(n)，支持动态扩容
 */
#ifndef SEQLIST_H
#define SEQLIST_H

#include "common.h"

#define LIST_INIT_SIZE  10   /* 初始容量 */
#define LIST_INCREMENT   5   /* 每次扩容增量 */

typedef struct {
    ElemType *data;   /* 存储数组指针 */
    int length;       /* 当前元素个数 */
    int capacity;     /* 数组容量 */
} SeqList;

/* 初始化空顺序表 */
Status SeqList_Init(SeqList *L);
/* 销毁顺序表，释放内存 */
Status SeqList_Destroy(SeqList *L);
/* 清空顺序表（保留容量，不释放内存） */
Status SeqList_Clear(SeqList *L);
/* 判空 */
bool   SeqList_Empty(const SeqList *L);
/* 获取长度 */
int    SeqList_Length(const SeqList *L);
/* 获取第i个元素（1<=i<=length） */
Status SeqList_Get(const SeqList *L, int i, ElemType *e);
/* 查找元素位置（返回位序，找不到返回0） */
int    SeqList_Locate(const SeqList *L, ElemType e, CompareFunc cmp);
/* 在第i个位置插入元素e */
Status SeqList_Insert(SeqList *L, int i, ElemType e);
/* 删除第i个元素，用e返回其值 */
Status SeqList_Delete(SeqList *L, int i, ElemType *e);
/* 遍历顺序表 */
Status SeqList_Traverse(const SeqList *L, VisitFunc visit);
/* 末尾追加元素（高效操作） */
Status SeqList_PushBack(SeqList *L, ElemType e);

#endif /* SEQLIST_H */
