#include <stdio.h>
#include <stdlib.h>

/*
* C 语言经典100例-77
* https://www.runoob.com/cprogramming/c-exercise-example77.html
* 题目：指针的指针
* 练习指向指针的指针（使用多级指针遍历字符串数组）。
*/
int main()
{
    printf("指针的指针，字符串指针\n");

    // 定义一个字符指针数组，存储字符串常量
    const char* s[] = { "man", "woman", "girl", "boy", "sister" };
    // 定义一个指向字符指针的指针变量
    const char** q;
    int k;

    // 遍历数组，并通过指向指针的指针输出每个字符串
    for (k = 0; k < 5; k++)
    {
        // 让指针 q 指向字符指针数组中第 k 个元素的地址
        q = &s[k];
        // 解引用 q，输出对应的字符串
        printf("%s\n", *q);
    }

    return 0;
}