#include <stdio.h>
#include <assert.h>
#include "seqlist.h"

void test_seqlist() {
    printf("===== 测试1：顺序表 SeqList =====\n");
    SeqList L;
    assert(SeqList_Init(&L) == OK);
    assert(SeqList_Empty(&L) == true);

    /* 尾部追加：10 20 30 40 50 */
    int init_data[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++)
        SeqList_PushBack(&L, init_data[i]);
    assert(SeqList_Length(&L) == 5);
    printf("追加5个元素后：");
    SeqList_Traverse(&L, print_int);  /* 期望: 10 20 30 40 50 */

    /* 在第3个位置插入99 */
    assert(SeqList_Insert(&L, 3, 99) == OK);
    printf("第3个位置插入99后：");
    SeqList_Traverse(&L, print_int);  /* 期望: 10 20 99 30 40 50 */

    /* 查找元素30 */
    int pos = SeqList_Locate(&L, 30, cmp_int);
    printf("元素30的位置：%d\n", pos);  /* 期望: 4 */
    assert(pos == 4);

    /* 获取第2个元素 */
    ElemType e;
    SeqList_Get(&L, 2, &e);
    printf("第2个元素：%d\n", e);       /* 期望: 20 */
    assert(e == 20);

    /* 删除第4个元素 */
    SeqList_Delete(&L, 4, &e);
    printf("删除第4个元素(%d)后：", e); /* 期望: 30 */
    SeqList_Traverse(&L, print_int);  /* 期望: 10 20 99 40 50 */

    /* 扩容测试：插入超过初始容量的元素 */
    printf("连续插入20个元素测试扩容：");
    for (int i = 0; i < 20; i++)
        SeqList_PushBack(&L, i);
    printf("长度=%d 容量=%d\n", L.length, L.capacity);
    assert(L.length == 25);

    SeqList_Clear(&L);
    assert(SeqList_Empty(&L));
    SeqList_Destroy(&L);
    printf("✅ 顺序表测试全部通过！\n\n");
}
