#include "linklist.h"

Status LinkList_Init(LinkList *L) {
    if (!L) return ERROR;
    *L = (LNode*)malloc(sizeof(LNode));
    if (!*L) return OVERFLOW;
    (*L)->next = NULL;
    return OK;
}

Status LinkList_Destroy(LinkList *L) {
    if (!L || !*L) return ERROR;
    LNode *p = *L, *q;
    while (p) {
        q = p->next;
        free(p);
        p = q;
    }
    *L = NULL;
    return OK;
}

Status LinkList_Clear(LinkList L) {
    if (!L) return ERROR;
    LNode *p = L->next, *q;
    while (p) {
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;
    return OK;
}

bool LinkList_Empty(LinkList L) {
    return L ? L->next == NULL : true;
}

int LinkList_Length(LinkList L) {
    if (!L) return 0;
    int len = 0;
    LNode *p = L->next;
    while (p) { len++; p = p->next; }
    return len;
}

/* 获取指向第i个节点的指针（内部工具，1<=i<=n） */
static LNode* get_node(LinkList L, int i) {
    LNode *p = L;
    int j = 0;
    while (p && j < i) { p = p->next; j++; }
    return p;  /* 找不到返回NULL */
}

Status LinkList_Get(LinkList L, int i, ElemType *e) {
    if (!L || !e || i < 1) return ERROR;
    LNode *p = get_node(L, i);
    if (!p) return ERROR;
    *e = p->data;
    return OK;
}

LNode* LinkList_Locate(LinkList L, ElemType e, CompareFunc cmp) {
    if (!L || !cmp) return NULL;
    LNode *p = L->next;
    while (p) {
        if (cmp(p->data, e) == 0) return p;
        p = p->next;
    }
    return NULL;
}

Status LinkList_Insert(LinkList L, int i, ElemType e) {
    /* 在第i个位置插入：找到第i-1个节点 */
    if (!L) return ERROR;
    LNode *p = get_node(L, i-1);
    if (!p) return ERROR;
    LNode *s = (LNode*)malloc(sizeof(LNode));
    if (!s) return OVERFLOW;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

Status LinkList_Delete(LinkList L, int i, ElemType *e) {
    if (!L) return ERROR;
    LNode *p = get_node(L, i-1);
    if (!p || !p->next) return ERROR;
    LNode *q = p->next;
    if (e) *e = q->data;
    p->next = q->next;
    free(q);
    return OK;
}

Status LinkList_Create_Head(LinkList *L, ElemType arr[], int n) {
    /* 头插法：新节点总是插入到头结点之后，结果逆序 */
    LinkList_Init(L);
    for (int i = 0; i < n; i++) {
        LNode *s = (LNode*)malloc(sizeof(LNode));
        if (!s) return OVERFLOW;
        s->data = arr[i];
        s->next = (*L)->next;
        (*L)->next = s;
    }
    return OK;
}

Status LinkList_Create_Tail(LinkList *L, ElemType arr[], int n) {
    /* 尾插法：使用尾指针，保证顺序 */
    LinkList_Init(L);
    LNode *tail = *L;  /* 尾指针初始指向头结点 */
    for (int i = 0; i < n; i++) {
        LNode *s = (LNode*)malloc(sizeof(LNode));
        if (!s) return OVERFLOW;
        s->data = arr[i];
        s->next = NULL;
        tail->next = s;
        tail = s;
    }
    return OK;
}

Status LinkList_Traverse(LinkList L, VisitFunc visit) {
    if (!L || !visit) return ERROR;
    LNode *p = L->next;
    while (p) { visit(p->data); p = p->next; }
    printf("\n");
    return OK;
}

/* ========== 进阶算法 ========== */

Status LinkList_Reverse(LinkList L) {
    /* 三指针原地反转 */
    if (!L || !L->next) return ERROR;
    LNode *pre = NULL, *cur = L->next, *next;
    while (cur) {
        next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    L->next = pre;
    return OK;
}

LNode* LinkList_FindLastK(LinkList L, int k) {
    /* 快指针先走k步，然后快慢同步，快到尾时慢即为倒数第k */
    if (!L || k < 1) return NULL;
    LNode *fast = L->next, *slow = L->next;
    for (int i = 0; i < k; i++) {
        if (!fast) return NULL;  /* 长度不足k */
        fast = fast->next;
    }
    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }
    return slow;
}

LNode* LinkList_DetectCycle(LinkList L) {
    /* Floyd龟兔赛跑算法 */
    if (!L || !L->next) return NULL;
    LNode *slow = L->next, *fast = L->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {  /* 相遇，有环 */
            LNode *p = L->next;
            while (p != slow) { p = p->next; slow = slow->next; }
            return p;  /* 入环节点 */
        }
    }
    return NULL;
}
