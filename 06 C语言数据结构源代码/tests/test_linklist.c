#include <stdio.h>
#include <assert.h>
#include "linklist.h"

void test_linklist() {
    printf("===== 测试2：单链表 LinkList =====\n");
    LinkList L;
    int arr[] = {1, 3, 5, 7, 9};
    assert(LinkList_Create_Tail(&L, arr, 5) == OK);
    assert(LinkList_Length(L) == 5);
    printf("尾插法建表：");
    LinkList_Traverse(L, print_int); /* 1 3 5 7 9 */

    assert(LinkList_Insert(L, 3, 666) == OK);
    printf("第3位插入666：");
    LinkList_Traverse(L, print_int); /* 1 3 666 5 7 9 */

    ElemType e;
    assert(LinkList_Delete(L, 2, &e) == OK);
    printf("删除第2位(%d)：", e);
    LinkList_Traverse(L, print_int); /* 1 666 5 7 9 */

    LNode *p = LinkList_Locate(L, 5, cmp_int);
    assert(p && p->data == 5);
    printf("元素5查找成功，节点地址=%p\n", (void*)p);

    /* 链表反转测试 */
    LinkList_Reverse(L);
    printf("反转后：");
    LinkList_Traverse(L, print_int); /* 9 7 5 666 1 */

    /* 倒数第k个节点测试 */
    p = LinkList_FindLastK(L, 2);
    assert(p && p->data == 666);
    printf("倒数第2个节点值：%d\n", p->data);

    /* 头插法测试（逆序） */
    LinkList L2;
    int arr2[] = {1,2,3,4};
    LinkList_Create_Head(&L2, arr2, 4);
    printf("头插法[1,2,3,4]结果：");
    LinkList_Traverse(L2, print_int); /* 4 3 2 1 */

    /* 环检测测试（无环） */
    assert(LinkList_DetectCycle(L) == NULL);
    printf("链表无环检测通过\n");

    LinkList_Destroy(&L);
    LinkList_Destroy(&L2);
    printf("✅ 单链表测试全部通过！\n\n");
}
