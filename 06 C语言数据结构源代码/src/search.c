#include "search.h"

int SeqSearch(ElemType arr[], int n, ElemType key) {
    /* 哨兵优化：arr[0]预留为哨兵位，从后往前找，无需边界判断 */
    /* 这里假设arr从1开始使用，n是有效长度，0号位为哨兵槽 */
    arr[0] = key;
    int i;
    for (i = n; arr[i] != key; i--);
    return i;  /* 0表示未找到 */
}

int BinSearch(ElemType arr[], int n, ElemType key) {
    /* arr[0..n-1]为升序有序数组，0-based */
    int low = 0, high = n - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (arr[mid] == key) return mid;
        else if (key < arr[mid]) high = mid - 1;
        else low = mid + 1;
    }
    return -1;
}

int InterpolationSearch(ElemType arr[], int n, ElemType key) {
    int low = 0, high = n - 1;
    while (low <= high && key >= arr[low] && key <= arr[high]) {
        if (low == high) return arr[low] == key ? low : -1;
        int mid = low + (int)((double)(key - arr[low]) / (arr[high] - arr[low]) * (high - low));
        if (arr[mid] == key) return mid;
        if (arr[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

/* ========== 散列表 ========== */
int HashFunc(ElemType key) {
    return key % HASH_TABLE_SIZE;
}

void HashInit(HashTable ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) ht[i] = NULL;
}

void HashDestroy(HashTable ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *p = ht[i], *q;
        while (p) { q = p->next; free(p); p = q; }
        ht[i] = NULL;
    }
}

bool HashInsert(HashTable ht, ElemType key) {
    int idx = HashFunc(key);
    /* 查重 */
    HashNode *p = ht[idx];
    while (p) {
        if (p->key == key) return false; /* 已存在 */
        p = p->next;
    }
    HashNode *s = (HashNode*)malloc(sizeof(HashNode));
    s->key = key;
    s->next = ht[idx];  /* 头插 */
    ht[idx] = s;
    return true;
}

HashNode* HashSearch(HashTable ht, ElemType key) {
    int idx = HashFunc(key);
    HashNode *p = ht[idx];
    while (p) {
        if (p->key == key) return p;
        p = p->next;
    }
    return NULL;
}

int HashDelete(HashTable ht, ElemType key) {
    int idx = HashFunc(key);
    HashNode *p = ht[idx], *pre = NULL;
    while (p) {
        if (p->key == key) {
            if (pre) pre->next = p->next;
            else ht[idx] = p->next;
            free(p);
            return 1;
        }
        pre = p; p = p->next;
    }
    return 0;
}
