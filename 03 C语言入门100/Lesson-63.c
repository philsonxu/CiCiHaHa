#include <stdio.h>

/*
* C 语言实例 - 复数相加
* https://www.runoob.com/cprogramming/c-examples.html
*/

typedef struct complex
{
    float real;
    float imag;
} complex;

complex complex_add(complex n1,complex n2);

int main()
{
    complex n1, n2, temp;

    printf("第一个复数 \n");
    printf("输入实部和虚部:\n");
    scanf("%f %f", &n1.real, &n1.imag);

    printf("\n第二个复数 \n");
    printf("输入实部和虚部:\n");
    scanf("%f %f", &n2.real, &n2.imag);

    temp = complex_add(n1, n2);
    printf("Sum = %.1f + %.1fi", temp.real, temp.imag);

    return 0;
}

/*
complex complex_add(complex n1, complex n2)
{
    complex temp;
    temp.real = n1.real + n2.real;
    temp.imag = n1.imag + n2.imag;
    return(temp);
}
*/