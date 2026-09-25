#include <stdio.h>
#include <assert.h>
#include "search.h"

void test_search() {
    printf("===== 测试8：查找算法 Search =====\n");

    /* 折半查找 */
    ElemType arr[] = {0, 3, 7, 12, 23, 37, 45, 68, 99}; /* 0位置给哨兵预留 */
    int n = 8;
    printf("有序数组(0位哨兵槽1~8有效): ");
    for (int i = 1; i <= n; i++) printf("%d ", arr[i]);
    printf("\n");

    /* SeqSearch 1-based: arr[1..8]对应值 3,7,12,23,37,45,68,99 */
    /* 45是第6个元素（1-based） */
    int idx = SeqSearch(arr, n, 45);
    printf("顺序查找45：下标=%d（0为哨兵槽，期望6）\n", idx);
    assert(idx == 6);
    assert(SeqSearch(arr, n, 100) == 0);  /* 未找到，返回哨兵槽0 */

    /* BinSearch 0-based，偏移1 */
    idx = BinSearch(arr+1, n, 45);
    printf("折半查找45：下标=%d（期望5，0-based）\n", idx);
    assert(idx == 5);
    assert(BinSearch(arr+1, n, 100) == -1);

    idx = InterpolationSearch(arr+1, n, 37);
    printf("插值查找37：下标=%d（期望4）\n", idx);
    assert(idx == 4);

    /* 散列表测试 */
    printf("\n--- 链地址法散列表 ---\n");
    HashTable ht;
    HashInit(ht);
    int keys[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    int kn = sizeof(keys)/sizeof(keys[0]);
    for (int i = 0; i < kn; i++) HashInsert(ht, keys[i]);

    /* 打印哈希桶 */
    printf("散列表桶状态（mod 13）：\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        printf("  [%2d]: ", i);
        HashNode *p = ht[i];
        while (p) { printf("%d -> ", p->key); p = p->next; }
        printf("NULL\n");
    }

    assert(HashSearch(ht, 68));
    assert(HashSearch(ht, 100) == NULL);
    printf("查找68：✅ 查找100：未找到(正确)\n");

    assert(HashDelete(ht, 23) == 1);
    assert(HashSearch(ht, 23) == NULL);
    printf("删除23后再次查找：未找到(正确)\n");

    /* 重复插入应失败 */
    assert(HashInsert(ht, 68) == false);
    printf("重复插入68失败（正确）\n");

    HashDestroy(ht);
    printf("✅ 查找测试全部通过！\n\n");
}
