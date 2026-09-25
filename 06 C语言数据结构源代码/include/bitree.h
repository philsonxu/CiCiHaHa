/**
 * 二叉树（二叉链表存储）
 * 包含：前/中/后/层序遍历，求深度、节点数、叶子数，复制、销毁，以及二叉搜索树BST
 */
#ifndef BITREE_H
#define BITREE_H

#include "common.h"

/* 扩展先序序列中用于表示空节点的标记（-1代表NULL） */
#define EMPTY_NODE -1

typedef struct BiTNode {
    ElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

/* 创建二叉树：从扩展先序序列（含空节点标记）递归构建 */
Status BiTree_Create(BiTree *T, ElemType arr[], int *idx);
/* 销毁整棵树 */
Status BiTree_Destroy(BiTree *T);
/* 递归前序遍历 */
void   BiTree_PreOrder(const BiTree T, VisitFunc visit);
/* 递归中序遍历 */
void   BiTree_InOrder(const BiTree T, VisitFunc visit);
/* 递归后序遍历 */
void   BiTree_PostOrder(const BiTree T, VisitFunc visit);
/* 层序遍历（借助队列） */
void   BiTree_LevelOrder(const BiTree T, VisitFunc visit);
/* 树的深度（高度） */
int    BiTree_Depth(const BiTree T);
/* 节点总数 */
int    BiTree_NodeCount(const BiTree T);
/* 叶子节点数 */
int    BiTree_LeafCount(const BiTree T);
/* 查找值为e的节点 */
BiTNode* BiTree_Search(const BiTree T, ElemType e);
/* 复制二叉树 */
BiTree BiTree_Copy(const BiTree T);

/* ========== 二叉搜索树 BST ========== */
/* BST查找（递归版） */
BiTNode* BST_Search(BiTree T, ElemType key);
/* BST插入 */
Status BST_Insert(BiTree *T, ElemType key);
/* BST删除 */
Status BST_Delete(BiTree *T, ElemType key);
/* 用数组构建BST */
Status BST_Create(BiTree *T, ElemType arr[], int n);

#endif /* BITREE_H */
