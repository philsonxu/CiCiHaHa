#include <stdio.h>

/*
* C 语言经典100例-11
* https://www.runoob.com/cprogramming/c-exercise-example12.html
* 题目：古典问题（兔子生崽）
* 有一对兔子，从出生后第3个月起每个月都生一对兔子，
* 小兔子长到第三个月后每个月又生一对兔子，
* 假如兔子都不死，问每个月的兔子总数为多少？（输出前20个月即可）
*/
int main()
{
    int a = 1, b = 1, temp, i;
    printf("兔子生崽问题（算法1）：\n");
    printf("%12d%12d", a, b); // 输出前两项

    for (i = 3; i <= 20; i++)// += 2)
    {
        temp = a + b; // 计算下一项
        printf("%12d", temp); // 输出下一项

        a = b; // 更新a
        b = temp; // 更新b

        temp = a + b; // 再计算下一项
        printf("%12d", temp); // 输出并换行

		if (i % 2 == 1) printf("\n");

        a = b; // 更新a
        b = temp; // 更新b
    }
    printf("\n"); 
    printf("\n");
    return 0;
}

/*
* C 语言经典100例-11
* https://www.w3cschool.cn/c/c-exercise-example11.html
*/
int main()
{
    printf("兔子生崽问题（算法2）：\n");
    int f1 = 1, f2 = 1, i;
    for (i = 1; i <= 20; i++)
    {
        printf("%12d%12d", f1, f2);
        if (i % 2 == 0) printf("\n");
        f1 = f1 + f2;
        f2 = f1 + f2;
    }

    return 0;
}