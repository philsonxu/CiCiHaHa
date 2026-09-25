#include "queue.h"

/* ========== 循环队列 ========== */
Status CircQueue_Init(CircQueue *Q) {
    if (!Q) return ERROR;
    Q->front = Q->rear = 0;
    return OK;
}
void CircQueue_Clear(CircQueue *Q) { if (Q) Q->front = Q->rear = 0; }
bool CircQueue_Empty(const CircQueue *Q) { return Q && Q->front == Q->rear; }
bool CircQueue_Full(const CircQueue *Q) { return Q && (Q->rear+1)%QUEUE_MAX_SIZE == Q->front; }
int  CircQueue_Length(const CircQueue *Q) {
    return Q ? (Q->rear - Q->front + QUEUE_MAX_SIZE) % QUEUE_MAX_SIZE : 0;
}
Status CircQueue_Enqueue(CircQueue *Q, ElemType e) {
    if (!Q || CircQueue_Full(Q)) return ERROR;
    Q->data[Q->rear] = e;
    Q->rear = (Q->rear + 1) % QUEUE_MAX_SIZE;
    return OK;
}
Status CircQueue_Dequeue(CircQueue *Q, ElemType *e) {
    if (!Q || CircQueue_Empty(Q)) return ERROR;
    if (e) *e = Q->data[Q->front];
    Q->front = (Q->front + 1) % QUEUE_MAX_SIZE;
    return OK;
}
Status CircQueue_Front(const CircQueue *Q, ElemType *e) {
    if (!Q || !e || CircQueue_Empty(Q)) return ERROR;
    *e = Q->data[Q->front];
    return OK;
}
Status CircQueue_Traverse(const CircQueue *Q, VisitFunc visit) {
    if (!Q || !visit) return ERROR;
    int i = Q->front;
    while (i != Q->rear) {
        visit(Q->data[i]);
        i = (i + 1) % QUEUE_MAX_SIZE;
    }
    printf("\n");
    return OK;
}

/* ========== 链式队列 ========== */
Status LinkQueue_Init(LinkQueue *Q) {
    if (!Q) return ERROR;
    Q->front = Q->rear = (QNode*)malloc(sizeof(QNode));
    if (!Q->front) return OVERFLOW;
    Q->front->next = NULL;
    Q->length = 0;
    return OK;
}
Status LinkQueue_Destroy(LinkQueue *Q) {
    if (!Q) return ERROR;
    while (Q->front) {
        Q->rear = Q->front->next;
        free(Q->front);
        Q->front = Q->rear;
    }
    Q->length = 0;
    return OK;
}
void LinkQueue_Clear(LinkQueue *Q) {
    if (!Q) return;
    QNode *p = Q->front->next, *q;
    while (p) { q = p->next; free(p); p = q; }
    Q->rear = Q->front;
    Q->front->next = NULL;
    Q->length = 0;
}
bool LinkQueue_Empty(const LinkQueue *Q) { return Q && Q->front == Q->rear; }
int  LinkQueue_Length(const LinkQueue *Q) { return Q ? Q->length : 0; }

Status LinkQueue_Enqueue(LinkQueue *Q, ElemType e) {
    if (!Q) return ERROR;
    QNode *s = (QNode*)malloc(sizeof(QNode));
    if (!s) return OVERFLOW;
    s->data = e; s->next = NULL;
    Q->rear->next = s;
    Q->rear = s;
    Q->length++;
    return OK;
}
Status LinkQueue_Dequeue(LinkQueue *Q, ElemType *e) {
    if (!Q || LinkQueue_Empty(Q)) return ERROR;
    QNode *p = Q->front->next;
    if (e) *e = p->data;
    Q->front->next = p->next;
    if (Q->rear == p) Q->rear = Q->front;  /* 删除最后一个节点 */
    free(p);
    Q->length--;
    return OK;
}
Status LinkQueue_Front(const LinkQueue *Q, ElemType *e) {
    if (!Q || !e || LinkQueue_Empty(Q)) return ERROR;
    *e = Q->front->next->data;
    return OK;
}
Status LinkQueue_Traverse(const LinkQueue *Q, VisitFunc visit) {
    if (!Q || !visit) return ERROR;
    QNode *p = Q->front->next;
    while (p) { visit(p->data); p = p->next; }
    printf("\n");
    return OK;
}
