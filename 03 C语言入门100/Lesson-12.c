#include <stdio.h>

/*
* C 语言实例 - 交换两个数的值
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    double firstNumber, secondNumber, temporaryVariable;

    printf("输入第一个数字: ");
    scanf("%lf", &firstNumber);

    printf("输入第二个数字: ");
    scanf("%lf", &secondNumber);

    // 将第一个数的值赋值给 temporaryVariable
    temporaryVariable = firstNumber;

    // 第二个数的值赋值给 firstNumber
    firstNumber = secondNumber;

    // 将 temporaryVariable 赋值给 secondNumber
    secondNumber = temporaryVariable;

    printf("\n交换后, firstNumber = %.2lf\n", firstNumber);
    printf("交换后, secondNumber = %.2lf", secondNumber);

    return 0;
}

int main()
{
    int a, b;

    a = 11;
    b = 99;

    printf("交换之前 - \n a = %d, b = %d \n\n", a, b);

    a = a + b;  // ( 11 + 99 = 110)  此时 a 的变量为两数之和，b 未改变
    b = a - b;  // ( 110 - 99 = 11)  
    a = a - b;  // ( 110 - 11 = 99)

    printf("交换后 - \n a = %d, b = %d \n", a, b);
}