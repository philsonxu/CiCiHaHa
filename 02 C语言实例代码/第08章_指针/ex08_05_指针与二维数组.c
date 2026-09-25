/*
 * 例8.5 指针与二维数组
 * 知识点：二维数组的地址理解（行地址、列地址）、a[i]等价于*(a+i)
 */
#include <stdio.h>

int main()
{
    int a[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    //int i, j;
    
    printf("二维数组a的地址分析：\n");
    printf("a = %p（第0行首地址）\n", a);
    printf("a+1 = %p（第1行首地址，步长为一行大小）\n", a + 1);
    printf("a[0] = %p（第0行第0列地址）\n", a[0]);
    printf("*a = %p（*a等价于a[0]）\n", *a);
    
    printf("\n用指针遍历二维数组：\n");
    int *p;
    for (p = a[0]; p < a[0] + 12; p++) {
        printf("%-4d", *p);
        if ((p - a[0] + 1) % 4 == 0)
            printf("\n");
    }
    
    return 0;
}