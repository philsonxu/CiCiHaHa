#include <stdio.h>
#include <assert.h>
#include "bitree.h"

void test_bitree() {
    printf("===== 测试6：二叉树 BiTree =====\n");

    /* 扩展先序序列构建：1 2 4 # # 5 # # 3 6 # # #  (#代表空，用-1表示)
       表示的树：
            1
           / \
          2   3
         / \ /
        4  5 6
    */
    ElemType arr[] = {1,2,4,-1,-1,5,-1,-1,3,6,-1,-1,-1};
    BiTree T;
    int idx = 0;
    BiTree_Create(&T, arr, &idx);

    printf("前序遍历："); BiTree_PreOrder(T, print_int); printf("\n");
    printf("中序遍历："); BiTree_InOrder(T, print_int); printf("\n");
    printf("后序遍历："); BiTree_PostOrder(T, print_int); printf("\n");
    printf("层序遍历："); BiTree_LevelOrder(T, print_int); printf("\n");

    printf("树深度：%d（期望3）\n", BiTree_Depth(T));
    printf("节点总数：%d（期望6）\n", BiTree_NodeCount(T));
    printf("叶子节点数：%d（期望3：4,5,6）\n", BiTree_LeafCount(T));
    assert(BiTree_Depth(T) == 3);
    assert(BiTree_NodeCount(T) == 6);
    assert(BiTree_LeafCount(T) == 3);

    BiTNode *p = BiTree_Search(T, 5);
    assert(p && p->data == 5);
    printf("查找节点5：找到\n");

    /* 复制并验证 */
    BiTree T2 = BiTree_Copy(T);
    printf("复制树的中序："); BiTree_InOrder(T2, print_int); printf("\n");
    assert(BiTree_NodeCount(T2) == 6);
    BiTree_Destroy(&T2);

    /* ========== BST 测试 ========== */
    printf("\n--- 二叉搜索树 BST ---\n");
    BiTree bst;
    int keys[] = {45, 24, 53, 12, 37, 93, 11, 30};
    BST_Create(&bst, keys, 8);
    printf("BST中序（应升序）：");
    BiTree_InOrder(bst, print_int); printf("\n");
    assert(BST_Search(bst, 37));
    assert(BST_Search(bst, 100) == NULL);
    printf("查找37：✅ 查找100：未找到（正确）\n");

    /* 删除叶子30 */
    BST_Delete(&bst, 30);
    printf("删除30后中序："); BiTree_InOrder(bst, print_int); printf("\n");
    /* 删除单子树节点12 */
    BST_Delete(&bst, 12);
    printf("删除12后中序："); BiTree_InOrder(bst, print_int); printf("\n");
    /* 删除双子树节点24 */
    BST_Delete(&bst, 24);
    printf("删除24后中序："); BiTree_InOrder(bst, print_int); printf("\n");
    assert(BST_Search(bst, 24) == NULL);

    BiTree_Destroy(&T);
    BiTree_Destroy(&bst);
    printf("✅ 二叉树测试全部通过！\n\n");
}
