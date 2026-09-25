/*
 * 例12.1 位运算符基础
 * 知识点：
 *   & 按位与 ：两位都为1则为1（常用于清0、取某些位）
 *   | 按位或 ：有一位为1则为1（常用于置位）
 *   ^ 按位异或：相同为0，不同为1（常用于翻转、交换两数）
 *   ~ 按位取反：0变1，1变0（单目运算符）
 *   << 左移  ：高位丢弃，低位补0（相当于乘2^n）
 *   >> 右移  ：低位丢弃，高位补0（逻辑右移）或补符号位（算术右移）
 */
#include <stdio.h>

void print_binary(int n)
{
    int i;
    for (i = 31; i >= 0; i--) {
        putchar((n >> i) & 1 ? '1' : '0');
        if (i % 8 == 0) putchar(' ');
    }
    printf("\n");
}

int main()
{
    unsigned char a = 0b10101010;  /* 170 */
    unsigned char b = 0b11001100;  /* 204 */
    
    printf("a = "); print_binary(a);
    printf("b = "); print_binary(b);
    
    printf("\n按位与 a&b = ");
    print_binary(a & b);  /* 10001000 */
    
    printf("按位或 a|b = ");
    print_binary(a | b);  /* 11101110 */
    
    printf("按位异或 a^b = ");
    print_binary(a ^ b);  /* 01100110 */
    
    printf("按位取反 ~a  = ");
    print_binary(~a);     /* 01010101 */
    
    printf("\n--- 移位运算 ---\n");
    unsigned char c = 0b00000011;  /* 3 */
    printf("c      = %d = ", c); print_binary(c);
    printf("c << 1 = %d = ", c << 1); print_binary(c << 1);  /* 左移1位=乘2=6 */
    printf("c << 2 = %d = ", c << 2); print_binary(c << 2);  /* 左移2位=乘4=12 */
    printf("c >> 1 = %d = ", c >> 1); print_binary(c >> 1);  /* 右移1位=除2=1 */
    
    return 0;
}