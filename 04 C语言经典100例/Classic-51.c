#include <stdio.h>
/*
#include <limits.h>

void printBinary(unsigned int num) {
    int bits = sizeof(int) * CHAR_BIT;
    for (int i = bits - 1; i >= 0; i--) {
        putchar((num >> i) & 1 ? '1' : '0');
        if (i % 4 == 0 && i != 0) putchar(' ');
    }
    printf(" %12u ", num);
    putchar('\n');
}

void printBinaryFormula(int a, int b, int c)
{
    printBinary(a);
    printBinary(b);
    printf("------------------------------------------\n");
    printBinary(c);
    printf("\n");
}
*/
void print_binary(int num);
void print_binary_formula(int a, int b, int c);

/*
* C 语言经典100例-51
* https://www.runoob.com/cprogramming/c-exercise-example51.html
* 题目：按位 与& 运算符
*/
int main()
{
    int a, b;
    a = 077;
    b = a & 3;
    printf("a & b(decimal) 为 %d \n", b);
    b &= 7;
    printf("a & b(decimal) 为 %d \n", b);
    return 0;
}


/*
* 改进版本
* 输出二进制格式的数字，看起来更直观！
*/
int main()
{
    int a = 077, b = 3;
    int c = a & b;
    print_binary_formula(a, b, c);

    a = c;
    b = 7;
    c = a & b;
    print_binary_formula(a, b, c);

    return 0;
}