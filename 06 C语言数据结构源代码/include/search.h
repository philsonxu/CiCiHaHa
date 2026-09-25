/**
 * 查找算法：顺序查找、折半查找（二分）、二叉搜索树查找、散列表查找
 */
#ifndef SEARCH_H
#define SEARCH_H

#include "common.h"

/* 顺序查找：线性遍历，返回下标(0-based)，找不到-1。可选哨兵优化 */
int SeqSearch(ElemType arr[], int n, ElemType key);
/* 折半查找：有序数组，返回下标(0-based)，找不到-1 */
int BinSearch(ElemType arr[], int n, ElemType key);
/* 插值查找：适合分布均匀的有序数组 */
int InterpolationSearch(ElemType arr[], int n, ElemType key);

/* ========== 散列表（链地址法）========== */
#define HASH_TABLE_SIZE 13
typedef struct HashNode {
    ElemType key;
    struct HashNode *next;
} HashNode, *HashTable[HASH_TABLE_SIZE];

void  HashInit(HashTable ht);
void  HashDestroy(HashTable ht);
int   HashFunc(ElemType key);
bool  HashInsert(HashTable ht, ElemType key);
HashNode* HashSearch(HashTable ht, ElemType key);
int   HashDelete(HashTable ht, ElemType key);

#endif /* SEARCH_H */
