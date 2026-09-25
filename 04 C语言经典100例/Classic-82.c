#include <stdio.h>
#include <math.h>
#include <string.h>

// 函数声明
int octal_to_decimal(const char* octalNumber);

/*
* C 语言经典100例-82
* https://www.runoob.com/cprogramming/c-exercise-example82.html
* 题目：八进制转换为十进制
*/
int main()
{
    char octalNumber[100];

    // 输入八进制数
    printf("八进制转换为十进制\n");
    printf("请输入一个八进制数: ");
    scanf("%s", octalNumber);

    // 转换为十进制
    int decimalNumber = octal_to_decimal(octalNumber);

    // 输出结果
    printf("八进制数 %s 转换为十进制数是: %d\n", octalNumber, decimalNumber);

    return 0;
}

// 八进制转换为十进制的函数
int octal_to_decimal(const char* octalNumber)
{
    int decimalNumber = 0;
    int length = strlen(octalNumber);

    for (int i = 0; i < length; i++) {
        int digit = octalNumber[i] - '0'; // 将字符转换为数字
        decimalNumber += digit * pow(8, length - i - 1);
    }

    return decimalNumber;
}