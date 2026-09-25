#include <stdio.h>

void print_result(long int b, long int i) 
{
    printf("\n%ld = 800 * %ld + 9 * %ld\n", b, i, i);
}

/*
* C 语言经典100例-81
* https://www.runoob.com/cprogramming/c-exercise-example81.html
* 题目：809问题
* 809 * ? ? = 800 * ? ? +9 * ? ? 其中 ? ? 代表的两位数, 
* 809 * ? ? 为四位数，8 * ? ? 的结果为两位数，
* 9 * ? ? 的结果为3位数。求 ? ? 代表的两位数，及809 * ? ? 后的结果。
*/
int main()
{
    printf("809问题\n");
    void output(long int b, long int i);
    long int a, b, i;
    a = 809;
    for (i = 10; i < 100; i++) {
        b = i * a;
        if (b >= 1000 && b <= 10000 && 8 * i < 100 && 9 * i >= 100) {
            print_result(b, i);
        }
    }
    return 0;
}