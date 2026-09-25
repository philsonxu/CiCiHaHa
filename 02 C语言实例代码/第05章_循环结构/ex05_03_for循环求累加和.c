/*
 * 例5.3 for循环 - 求1到100的和
 * 知识点：for循环结构（表达式1初始化;表达式2条件;表达式3更新）
 * for循环执行顺序：初始化 -> 判断条件 -> 执行循环体 -> 更新变量 -> 再判断
 */
#include <stdio.h>

int main()
{
    int i, sum = 0;
    for (i = 1; i <= 100; i++) {
        sum = sum + i;
    }
    printf("1+2+...+100 = %d\n", sum);
    
    /* for循环的灵活写法 */
    int s = 0;
    for (int k = 1; k <= 100; s += k, k++);  /* 空循环体 */
    printf("另一种写法结果：%d\n", s);
    
    /* 逗号表达式 */
    int a, b;
    for (a = 0, b = 10; a < b; a++, b--) {
        printf("a=%d, b=%d\n", a, b);
    }
    return 0;
}