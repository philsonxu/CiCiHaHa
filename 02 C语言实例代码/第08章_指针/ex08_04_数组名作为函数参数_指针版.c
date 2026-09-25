/*
 * 例8.4 数组名做形参本质上是指针
 * 知识点：形参int arr[]等价于int *arr
 */
#include <stdio.h>

/* 求数组元素和，参数写成指针形式 */
int sum(int *arr, int n)
{
    int i, s = 0;
    for (i = 0; i < n; i++) {
        s += *(arr + i);
    }
    return s;
}

int main()
{
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("数组元素之和：%d\n", sum(a, 10));
    return 0;
}