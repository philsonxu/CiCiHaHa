#include "seqlist.h"

Status SeqList_Init(SeqList *L) {
    if (!L) return ERROR;
    L->data = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
    if (!L->data) return OVERFLOW;
    L->length = 0;
    L->capacity = LIST_INIT_SIZE;
    return OK;
}

Status SeqList_Destroy(SeqList *L) {
    if (!L || !L->data) return ERROR;
    free(L->data);
    L->data = NULL;
    L->length = L->capacity = 0;
    return OK;
}

Status SeqList_Clear(SeqList *L) {
    if (!L) return ERROR;
    L->length = 0;
    return OK;
}

bool SeqList_Empty(const SeqList *L) {
    return L ? L->length == 0 : true;
}

int SeqList_Length(const SeqList *L) {
    return L ? L->length : 0;
}

/* 扩容：容量不足时调用 */
static Status ensure_capacity(SeqList *L) {
    if (L->length >= L->capacity) {
        int new_cap = L->capacity + LIST_INCREMENT;
        ElemType *new_base = (ElemType*)realloc(L->data, new_cap * sizeof(ElemType));
        if (!new_base) return OVERFLOW;
        L->data = new_base;
        L->capacity = new_cap;
    }
    return OK;
}

Status SeqList_Get(const SeqList *L, int i, ElemType *e) {
    if (!L || !e || i < 1 || i > L->length) return ERROR;
    *e = L->data[i-1];
    return OK;
}

int SeqList_Locate(const SeqList *L, ElemType e, CompareFunc cmp) {
    if (!L || !cmp) return 0;
    for (int i = 0; i < L->length; i++) {
        if (cmp(L->data[i], e) == 0)
            return i + 1;  /* 返回1-based位序 */
    }
    return 0;
}

Status SeqList_Insert(SeqList *L, int i, ElemType e) {
    if (!L) return ERROR;
    if (i < 1 || i > L->length + 1) return ERROR;
    if (ensure_capacity(L) != OK) return OVERFLOW;
    /* 从最后一个元素开始后移，为新元素腾出位置 */
    for (int j = L->length; j >= i; j--)
        L->data[j] = L->data[j-1];
    L->data[i-1] = e;
    L->length++;
    return OK;
}

Status SeqList_Delete(SeqList *L, int i, ElemType *e) {
    if (!L) return ERROR;
    if (i < 1 || i > L->length) return ERROR;
    if (e) *e = L->data[i-1];
    /* 元素前移覆盖 */
    for (int j = i; j < L->length; j++)
        L->data[j-1] = L->data[j];
    L->length--;
    return OK;
}

Status SeqList_Traverse(const SeqList *L, VisitFunc visit) {
    if (!L || !visit) return ERROR;
    for (int i = 0; i < L->length; i++)
        visit(L->data[i]);
    printf("\n");
    return OK;
}

Status SeqList_PushBack(SeqList *L, ElemType e) {
    if (!L) return ERROR;
    if (ensure_capacity(L) != OK) return OVERFLOW;
    L->data[L->length++] = e;
    return OK;
}
