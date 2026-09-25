#include <stdio.h>

#define N 10

void selection_sort_int(int arr[], int n) 
{
    int i, j, minIndex, temp;
    for (i = 0; i < n - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            // Swap the elements
            temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

/*
* C 语言经典100例-37
* https://www.runoob.com/cprogramming/c-exercise-example37.html
* 题目：对10个数进行排序。
*/
int main()
{
    int arr[N];
    printf("对10个数进行排序\n");
    printf("请输入 %d 个数字：\n", N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &arr[i]);
    }

    selection_sort_int(arr, N);

    printf("排序结果是:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}