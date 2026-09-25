/*
 * 例8.3 指针与一维数组
 * 知识点：数组名是数组首元素地址、指针遍历数组、指针算术运算
 */
#include <stdio.h>

int main()
{
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *p, i;
    
    /* 三种等价的访问方式 */
    printf("方式1：下标法 a[i]\n");
    for (i = 0; i < 10; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    
    printf("方式2：地址法 *(a+i)\n");
    for (i = 0; i < 10; i++) {
        printf("%d ", *(a + i));
    }
    printf("\n");
    
    printf("方式3：指针法 *p\n");
    for (p = a; p < a + 10; p++) {
        printf("%d ", *p);
    }
    printf("\n");
    
    return 0;
}