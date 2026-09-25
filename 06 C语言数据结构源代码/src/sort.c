#include "sort.h"

bool IsSorted(ElemType arr[], int n) {
    for (int i = 1; i < n; i++)
        if (arr[i] > arr[i+1]) return false;
    return true;
}

void ArrayCopy(ElemType dst[], ElemType src[], int n) {
    for (int i = 0; i <= n; i++) dst[i] = src[i]; /* 0号位也拷贝（哨兵） */
}

/* ========== 插入排序 ========== */
void InsertSort(ElemType arr[], int n) {
    for (int i = 2; i <= n; i++) {
        if (arr[i] < arr[i-1]) {
            arr[0] = arr[i];  /* 哨兵 */
            int j;
            for (j = i-1; arr[j] > arr[0]; j--)
                arr[j+1] = arr[j];
            arr[j+1] = arr[0];
        }
    }
}

void BinInsertSort(ElemType arr[], int n) {
    for (int i = 2; i <= n; i++) {
        arr[0] = arr[i];
        int lo = 1, hi = i-1;
        while (lo <= hi) {
            int mid = (lo+hi)/2;
            if (arr[mid] > arr[0]) hi = mid - 1;
            else lo = mid + 1;
        }
        for (int j = i-1; j >= lo; j--) arr[j+1] = arr[j];
        arr[lo] = arr[0];
    }
}

void ShellSort(ElemType arr[], int n) {
    for (int dk = n/2; dk >= 1; dk /= 2) {
        for (int i = dk+1; i <= n; i++) {
            if (arr[i] < arr[i-dk]) {
                arr[0] = arr[i];
                int j;
                for (j = i-dk; j > 0 && arr[j] > arr[0]; j -= dk)
                    arr[j+dk] = arr[j];
                arr[j+dk] = arr[0];
            }
        }
    }
}

/* ========== 交换排序 ========== */
void BubbleSort(ElemType arr[], int n) {
    for (int i = 1; i < n; i++) {
        bool swapped = false;
        for (int j = 1; j <= n - i; j++) {
            if (arr[j] > arr[j+1]) {
                ElemType t; SWAP(arr[j], arr[j+1], t);
                swapped = true;
            }
        }
        if (!swapped) break; /* 已排序，提前结束 */
    }
}

static int partition(ElemType arr[], int lo, int hi) {
    /* 随机选pivot并换到首位，使用经典Hoare双向扫描 */
    int mid = lo + (hi - lo) / 2;
    /* 三数取中：将中位数交换到lo位置作为pivot */
    if (arr[mid] < arr[lo]) { ElemType t; SWAP(arr[mid], arr[lo], t); }
    if (arr[hi]  < arr[lo]) { ElemType t; SWAP(arr[hi],  arr[lo], t); }
    if (arr[hi]  < arr[mid]){ ElemType t; SWAP(arr[hi],  arr[mid], t); }
    ElemType pivot = arr[lo];
    while (lo < hi) {
        while (lo < hi && arr[hi] >= pivot) hi--;
        arr[lo] = arr[hi];
        while (lo < hi && arr[lo] <= pivot) lo++;
        arr[hi] = arr[lo];
    }
    arr[lo] = pivot;
    return lo;
}

static void qs(ElemType arr[], int lo, int hi) {
    if (lo < hi) {
        int pivot = partition(arr, lo, hi);
        qs(arr, lo, pivot-1);
        qs(arr, pivot+1, hi);
    }
}

void QuickSort(ElemType arr[], int n) {
    qs(arr, 1, n);
}

/* ========== 选择排序 ========== */
void SelectSort(ElemType arr[], int n) {
    for (int i = 1; i < n; i++) {
        int min = i;
        for (int j = i+1; j <= n; j++)
            if (arr[j] < arr[min]) min = j;
        if (min != i) { ElemType t; SWAP(arr[i], arr[min], t); }
    }
}

/* 大根堆调整：下沉 */
static void heap_adjust(ElemType arr[], int k, int len) {
    arr[0] = arr[k];
    for (int i = 2*k; i <= len; i *= 2) {
        if (i < len && arr[i] < arr[i+1]) i++;
        if (arr[0] >= arr[i]) break;
        arr[k] = arr[i]; k = i;
    }
    arr[k] = arr[0];
}

void HeapSort(ElemType arr[], int n) {
    /* 建堆 */
    for (int i = n/2; i >= 1; i--)
        heap_adjust(arr, i, n);
    /* 依次取堆顶 */
    for (int i = n; i > 1; i--) {
        ElemType t; SWAP(arr[1], arr[i], t);
        heap_adjust(arr, 1, i-1);
    }
}

/* ========== 归并排序 ========== */
static void ms(ElemType arr[], ElemType tmp[], int lo, int hi) {
    if (lo >= hi) return;
    int mid = (lo+hi)/2;
    ms(arr, tmp, lo, mid);
    ms(arr, tmp, mid+1, hi);
    /* 合并 */
    int i = lo, j = mid+1, k = lo;
    while (i <= mid && j <= hi)
        tmp[k++] = arr[i] <= arr[j] ? arr[i++] : arr[j++];
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= hi)  tmp[k++] = arr[j++];
    for (i = lo; i <= hi; i++) arr[i] = tmp[i];
}

void MergeSort(ElemType arr[], int n) {
    ElemType *tmp = (ElemType*)malloc((n+1)*sizeof(ElemType));
    ms(arr, tmp, 1, n);
    free(tmp);
}

/* ========== 基数排序LSD（仅非负int） ========== */
void RadixSort(int arr[], int n) {
    /* arr[0..n-1] 0-based */
    int max = arr[0];
    for (int i = 1; i < n; i++) if (arr[i] > max) max = arr[i];
    int *buckets[10];
    for (int i = 0; i < 10; i++)
        buckets[i] = (int*)malloc(n * sizeof(int));
    int cnt[10];
    for (long long exp = 1; max / exp > 0; exp *= 10) {
        memset(cnt, 0, sizeof(cnt));
        for (int i = 0; i < n; i++) {
            int d = (int)((arr[i] / exp) % 10);
            buckets[d][cnt[d]++] = arr[i];
        }
        int k = 0;
        for (int d = 0; d < 10; d++)
            for (int i = 0; i < cnt[d]; i++)
                arr[k++] = buckets[d][i];
    }
    for (int i = 0; i < 10; i++) free(buckets[i]);
}

/* ========== 性能基准测试 ========== */
void BenchmarkAllSorts(int size) {
    printf("--- 排序性能基准（N=%d，随机整数）---\n", size);
    int *base = (int*)malloc((size+1)*sizeof(int));
    int *work = (int*)malloc((size+1)*sizeof(int));
    srand(42);
    for (int i = 1; i <= size; i++) base[i] = rand();

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
        ArrayCopy(work, base, size);
        clock_t t0 = clock();
        algos[i].fn(work, size);
        clock_t t1 = clock();
        double ms = (double)(t1 - t0) / CLOCKS_PER_SEC * 1000;
        printf("  %-12s %8.2f ms  %s\n", algos[i].name, ms,
               IsSorted(work, size) ? "✅" : "❌");
    }

    /* RadixSort（0-based，特殊处理） */
    int *arr0 = (int*)malloc(size*sizeof(int));
    for (int i = 0; i < size; i++) arr0[i] = base[i+1];
    clock_t t0 = clock();
    RadixSort(arr0, size);
    clock_t t1 = clock();
    double ms = (double)(t1 - t0) / CLOCKS_PER_SEC * 1000;
    bool ok = true;
    for (int i = 1; i < size; i++) if (arr0[i] < arr0[i-1]) ok = false;
    printf("  %-12s %8.2f ms  %s\n", "RadixSort", ms, ok?"✅":"❌");

    free(base); free(work); free(arr0);
}
