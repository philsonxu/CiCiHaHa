#include <stdio.h>

/*
* C 语言实例 - 计算 int, float, double 和 char 字节大小
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int integerType;
    float floatType;
    double doubleType;
    char charType;

    // sizeof 操作符用于计算变量的字节大小
    printf("各种类型变量占用字节数：\n");
    printf("Size of int: %ld bytes\n", sizeof(integerType));
    printf("Size of float: %ld bytes\n", sizeof(floatType));
    printf("Size of double: %ld bytes\n", sizeof(doubleType));
    printf("Size of char: %ld byte\n", sizeof(charType));

    return 0;
}

int main()
{
    int a;
    long b;
    long long c;

    double e;
    long double f;

    printf("各种类型变量占用字节数：\n");
    printf("Size of int = %ld bytes \n", sizeof(a));
    printf("Size of long = %ld bytes\n", sizeof(b));
    printf("Size of long long = %ld bytes\n", sizeof(c));

    printf("Size of double = %ld bytes\n", sizeof(e));
    printf("Size of long double = %ld bytes\n", sizeof(f));

    return 0;
}