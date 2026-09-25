/**
 * 循环队列（顺序存储）+ 链式队列
 */
#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

/* ========== 循环队列 ========== */
#define QUEUE_MAX_SIZE 20  /* 循环队列最多存 size-1 个元素 */

typedef struct {
    ElemType data[QUEUE_MAX_SIZE];
    int front;  /* 队头，指向队头元素 */
    int rear;   /* 队尾，指向下一个可插入位置 */
} CircQueue;

Status CircQueue_Init(CircQueue *Q);
void   CircQueue_Clear(CircQueue *Q);
bool   CircQueue_Empty(const CircQueue *Q);
bool   CircQueue_Full(const CircQueue *Q);
int    CircQueue_Length(const CircQueue *Q);
Status CircQueue_Enqueue(CircQueue *Q, ElemType e);
Status CircQueue_Dequeue(CircQueue *Q, ElemType *e);
Status CircQueue_Front(const CircQueue *Q, ElemType *e);
Status CircQueue_Traverse(const CircQueue *Q, VisitFunc visit);

/* ========== 链式队列 ========== */
typedef struct QNode {
    ElemType data;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front;   /* 队头指针 */
    QNode *rear;    /* 队尾指针 */
    int length;
} LinkQueue;

Status LinkQueue_Init(LinkQueue *Q);
Status LinkQueue_Destroy(LinkQueue *Q);
void   LinkQueue_Clear(LinkQueue *Q);
bool   LinkQueue_Empty(const LinkQueue *Q);
int    LinkQueue_Length(const LinkQueue *Q);
Status LinkQueue_Enqueue(LinkQueue *Q, ElemType e);
Status LinkQueue_Dequeue(LinkQueue *Q, ElemType *e);
Status LinkQueue_Front(const LinkQueue *Q, ElemType *e);
Status LinkQueue_Traverse(const LinkQueue *Q, VisitFunc visit);

#endif /* QUEUE_H */
