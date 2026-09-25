#include "bitree.h"
#include "queue.h"
#include <stdint.h>

Status BiTree_Create(BiTree *T, ElemType arr[], int *idx) {
    ElemType ch = arr[(*idx)++];
    if (ch == EMPTY_NODE) {
        *T = NULL;
        return OK;
    }
    *T = (BiTNode*)malloc(sizeof(BiTNode));
    if (!*T) return OVERFLOW;
    (*T)->data = ch;
    BiTree_Create(&(*T)->lchild, arr, idx);
    BiTree_Create(&(*T)->rchild, arr, idx);
    return OK;
}

Status BiTree_Destroy(BiTree *T) {
    if (!T || !*T) return OK;
    BiTree_Destroy(&(*T)->lchild);
    BiTree_Destroy(&(*T)->rchild);
    free(*T);
    *T = NULL;
    return OK;
}

void BiTree_PreOrder(const BiTree T, VisitFunc visit) {
    if (T) {
        visit(T->data);
        BiTree_PreOrder(T->lchild, visit);
        BiTree_PreOrder(T->rchild, visit);
    }
}

void BiTree_InOrder(const BiTree T, VisitFunc visit) {
    if (T) {
        BiTree_InOrder(T->lchild, visit);
        visit(T->data);
        BiTree_InOrder(T->rchild, visit);
    }
}

void BiTree_PostOrder(const BiTree T, VisitFunc visit) {
    if (T) {
        BiTree_PostOrder(T->lchild, visit);
        BiTree_PostOrder(T->rchild, visit);
        visit(T->data);
    }
}

/* 内部简单指针队列（用于层序遍历，避免int截断指针） */
typedef struct PtrQNode { void *p; struct PtrQNode *next; } PtrQNode;
typedef struct { PtrQNode *front, *rear; } PtrQueue;
static void pq_init(PtrQueue *q) { q->front = q->rear = (PtrQNode*)malloc(sizeof(PtrQNode)); q->front->next=NULL; }
static void pq_enq(PtrQueue *q, void *p) { PtrQNode *s=(PtrQNode*)malloc(sizeof(PtrQNode)); s->p=p; s->next=NULL; q->rear->next=s; q->rear=s; }
static void* pq_deq(PtrQueue *q) { PtrQNode *t=q->front->next; void *p=t->p; q->front->next=t->next; if(q->rear==t)q->rear=q->front; free(t); return p; }
static int pq_empty(PtrQueue *q) { return q->front==q->rear; }
static void pq_destroy(PtrQueue *q) { while(q->front){PtrQNode*t=q->front->next;free(q->front);q->front=t;} }

/* 层序遍历 */
void BiTree_LevelOrder(const BiTree T, VisitFunc visit) {
    if (!T) return;
    PtrQueue Q; pq_init(&Q);
    pq_enq(&Q, (void*)T);
    while (!pq_empty(&Q)) {
        BiTNode *p = (BiTNode*)pq_deq(&Q);
        visit(p->data);
        if (p->lchild) pq_enq(&Q, p->lchild);
        if (p->rchild) pq_enq(&Q, p->rchild);
    }
    pq_destroy(&Q);
}

int BiTree_Depth(const BiTree T) {
    if (!T) return 0;
    int ld = BiTree_Depth(T->lchild);
    int rd = BiTree_Depth(T->rchild);
    return MAX(ld, rd) + 1;
}

int BiTree_NodeCount(const BiTree T) {
    if (!T) return 0;
    return 1 + BiTree_NodeCount(T->lchild) + BiTree_NodeCount(T->rchild);
}

int BiTree_LeafCount(const BiTree T) {
    if (!T) return 0;
    if (!T->lchild && !T->rchild) return 1;
    return BiTree_LeafCount(T->lchild) + BiTree_LeafCount(T->rchild);
}

BiTNode* BiTree_Search(const BiTree T, ElemType e) {
    if (!T) return NULL;
    if (T->data == e) return (BiTNode*)T;
    BiTNode *p = BiTree_Search(T->lchild, e);
    if (p) return p;
    return BiTree_Search(T->rchild, e);
}

BiTree BiTree_Copy(const BiTree T) {
    if (!T) return NULL;
    BiTNode *p = (BiTNode*)malloc(sizeof(BiTNode));
    p->data = T->data;
    p->lchild = BiTree_Copy(T->lchild);
    p->rchild = BiTree_Copy(T->rchild);
    return p;
}

/* ========== 二叉搜索树 BST ========== */
BiTNode* BST_Search(BiTree T, ElemType key) {
    while (T) {
        if (key == T->data) return T;
        T = key < T->data ? T->lchild : T->rchild;
    }
    return NULL;
}

Status BST_Insert(BiTree *T, ElemType key) {
    if (!*T) {
        *T = (BiTNode*)malloc(sizeof(BiTNode));
        (*T)->data = key; (*T)->lchild = (*T)->rchild = NULL;
        return OK;
    }
    if (key == (*T)->data) return ERROR; /* BST不允许重复键 */
    if (key < (*T)->data)
        return BST_Insert(&(*T)->lchild, key);
    else
        return BST_Insert(&(*T)->rchild, key);
}

Status BST_Create(BiTree *T, ElemType arr[], int n) {
    *T = NULL;
    for (int i = 0; i < n; i++)
        BST_Insert(T, arr[i]);
    return OK;
}

Status BST_Delete(BiTree *T, ElemType key) {
    /* 经典三种情况：叶子单子树找后继 */
    if (!*T) return ERROR;
    if (key < (*T)->data) return BST_Delete(&(*T)->lchild, key);
    if (key > (*T)->data) return BST_Delete(&(*T)->rchild, key);
    /* 找到待删节点 */
    BiTNode *p = *T, *q;
    if (!p->rchild) {              /* 无右子树：用左孩子顶替 */
        *T = p->lchild; free(p);
    } else if (!p->lchild) {       /* 无左子树：用右孩子顶替 */
        *T = p->rchild; free(p);
    } else {                       /* 两子树均有：找中序后继（右子树最左） */
        q = p->rchild;
        while (q->lchild) q = q->lchild;
        p->data = q->data;         /* 用后继值覆盖 */
        BST_Delete(&p->rchild, q->data);  /* 在右子树中递归删除后继 */
    }
    return OK;
}
