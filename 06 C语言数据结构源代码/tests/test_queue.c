#include <stdio.h>
#include <assert.h>
#include "queue.h"

static void test_circqueue() {
    printf("--- 循环队列 ---\n");
    CircQueue Q;
    CircQueue_Init(&Q);
    assert(CircQueue_Empty(&Q));

    for (int i = 1; i <= 10; i++)
        CircQueue_Enqueue(&Q, i);
    assert(CircQueue_Length(&Q) == 10);
    printf("入队1~10：");
    CircQueue_Traverse(&Q, print_int);

    ElemType e;
    CircQueue_Dequeue(&Q, &e);
    CircQueue_Dequeue(&Q, &e);
    printf("出队2次(出队值依次应为1,2)，第二个出队：%d\n", e);
    assert(e == 2);
    printf("剩余元素：");
    CircQueue_Traverse(&Q, print_int);  /* 3 4 5 6 7 8 9 10 */

    /* 循环特性测试：再入队多个元素，测试取模 */
    for (int i = 100; i <= 110; i++)
        CircQueue_Enqueue(&Q, i);
    printf("再入队100~110后长度：%d\n", CircQueue_Length(&Q));
    printf("队头：");
    CircQueue_Front(&Q, &e);
    printf("%d\n", e);
    assert(e == 3);
}

static void test_linkqueue() {
    printf("--- 链式队列 ---\n");
    LinkQueue Q;
    LinkQueue_Init(&Q);
    for (int i = 1; i <= 5; i++)
        LinkQueue_Enqueue(&Q, i * 11);
    printf("入队 11 22 33 44 55：");
    LinkQueue_Traverse(&Q, print_int);

    ElemType e;
    LinkQueue_Dequeue(&Q, &e);
    printf("出队：%d，剩余：", e);
    assert(e == 11);
    LinkQueue_Traverse(&Q, print_int);
    assert(LinkQueue_Length(&Q) == 4);

    /* 全部出队，清空测试 */
    while (!LinkQueue_Empty(&Q))
        LinkQueue_Dequeue(&Q, NULL);
    assert(LinkQueue_Empty(&Q));
    printf("全部出队后判空：✅\n");

    LinkQueue_Destroy(&Q);
}

void test_queue() {
    printf("===== 测试4：队列 Queue =====\n");
    test_circqueue();
    test_linkqueue();
    printf("✅ 队列测试全部通过！\n\n");
}
