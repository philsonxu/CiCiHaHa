#include <stdio.h>

/*
* C 语言经典100例-05
* https://www.runoob.com/cprogramming/c-exercise-example5.html
* 题目：输入三个整数，由小到大输出。
*/
int main()
{
    int x, y, z, t;
    printf("\n请输入三个数字:\n");
    scanf("%d %d %d", &x, &y, &z);

    if (x > y) {
        /*交换x,y的值*/
        t = x; x = y; y = t;
    }

    if (x > z) {
        /*交换x,z的值*/
        t = z; z = x; x = t;
    }

    if (y > z) {
        /*交换z,y的值*/
        t = y; y = z; z = t;
    }

    printf("从小到大排序: %d %d %d\n", x, y, z);

    return 0;
}


// 交换两个变量的值
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


/*=====================================================================*/

int main()
{
    int a, b, c;
    printf("\n请输入三个数字:\n");
    scanf("%d %d %d", &a, &b, &c);

    if (a > b) {
        swap(&a, &b);
    }
    if (a > c) {
        swap(&a, &c);
    }
    if (b > c) {
        swap(&b, &c);
    }

    printf("从小到大排序: %d %d %d\n", a, b, c);
    return 0;
}