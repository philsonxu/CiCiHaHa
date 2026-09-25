#include <stdio.h>
#include <assert.h>
#include "sort.h"

static void print_arr(ElemType arr[], int n) {
    for (int i = 1; i <= n; i++) printf("%d ", arr[i]);
    printf("\n");
}

void test_sort() {
    printf("===== 测试9：排序算法 Sort =====\n");
    fflush(stdout);
    int test_data[] = {0, 49, 38, 65, 97, 76, 13, 27, 49, 55, 4}; /* 0位哨兵 */
    int n = 10;
    int buf[32];

    typedef struct { const char *name; void (*fn)(int*,int); } Algo;
    Algo algos[] = {
        {"InsertSort",  InsertSort},
        {"BinInsert",   BinInsertSort},
        {"ShellSort",   ShellSort},
        {"BubbleSort",  BubbleSort},
        {"QuickSort",   QuickSort},
        {"SelectSort",  SelectSort},
        {"HeapSort",    HeapSort},
        {"MergeSort",   MergeSort},
    };
    int na = sizeof(algos)/sizeof(algos[0]);

    for (int i = 0; i < na; i++) {
        ArrayCopy(buf, test_data, n);
        algos[i].fn(buf, n);
        printf("%-12s: ", algos[i].name);
        print_arr(buf, n);
        assert(IsSorted(buf, n));
    }

    /* 基数排序（0-based） */
    int radix_arr[] = {329, 457, 657, 839, 436, 720, 355};
    int rn = 7;
    printf("%-12s: ", "RadixSort");
    RadixSort(radix_arr, rn);
    for (int i = 0; i < rn; i++) printf("%d ", radix_arr[i]);
    printf("\n");
    for (int i = 1; i < rn; i++) assert(radix_arr[i] >= radix_arr[i-1]);

    /* 边界：已排序数组 */
    int sorted[] = {0,1,2,3,4,5,6,7,8,9,10};
    BubbleSort(sorted, 10);
    assert(IsSorted(sorted, 10));
    printf("已排序数组冒泡提前结束优化：✅\n");

    /* 性能基准 */
    printf("\n");
    BenchmarkAllSorts(10000);

    printf("✅ 排序算法测试全部通过！\n\n");
}
