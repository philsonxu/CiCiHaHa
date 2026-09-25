/*
 * 例7.6 数组作为函数参数
 * 知识点：数组名做参数传递的是首地址（地址传递），形参数组改变会影响实参数组
 */
#include <stdio.h>

/* 对数组进行冒泡排序 */
void bubble_sort(int arr[], int n)
{
    int i, j, t;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }
}

int main()
{
    int a[10] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    int i;
    
    printf("排序前：");
    for (i = 0; i < 10; i++)
        printf("%d ", a[i]);
    printf("\n");
    
    bubble_sort(a, 10);  /* 数组名作为实参 */
    
    printf("排序后：");
    for (i = 0; i < 10; i++)
        printf("%d ", a[i]);
    printf("\n");
    
    return 0;
}