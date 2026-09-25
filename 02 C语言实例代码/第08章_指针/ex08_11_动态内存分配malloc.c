/*
 * 例8.11 动态内存分配 malloc/free
 * 知识点：stdlib.h、malloc分配内存、free释放内存、防止内存泄漏
 */
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, i;
    int *arr;
    
    printf("请输入数组长度：");
    scanf("%d", &n);
    
    /* 动态分配n个int大小的内存 */
    arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("内存分配失败！\n");
        return 1;
    }
    
    /* 使用动态数组 */
    for (i = 0; i < n; i++) {
        arr[i] = i * i;
    }
    
    printf("动态数组内容：");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    free(arr);  /* 释放内存，必须与malloc配对使用 */
    arr = NULL; /* 防止野指针 */
    
    return 0;
}