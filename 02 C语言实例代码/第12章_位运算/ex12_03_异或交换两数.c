/*
 * 例12.3 用异或运算交换两个数（不借助临时变量）
 * 知识点：^异或运算的性质
 *   a ^ a = 0
 *   a ^ 0 = a
 *   a ^ b ^ b = a
 */
#include <stdio.h>

int main()
{
    int a = 3, b = 5;
    printf("交换前：a=%d, b=%d\n", a, b);
    
    /* 异或法交换 */
    a = a ^ b;
    b = a ^ b;  /* b = (a^b) ^ b = a */
    a = a ^ b;  /* a = (a^b) ^ a = b */
    
    printf("交换后：a=%d, b=%d\n", a, b);
    
    return 0;
}