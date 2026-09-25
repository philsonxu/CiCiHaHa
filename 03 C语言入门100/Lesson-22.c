#include<stdio.h> 

/*
* C 语言实例 - 输出九九乘法口诀表
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    //外层循环变量,控制行  
    int i = 0;
    //内层循环变量,控制列   
    int j = 0;
    for (i = 1; i <= 9; i++) {
        for (j = 1; j <= i; j++) {
            printf("%dx%d=%d\t", j, i, i * j);
        }
        //每行输出完后换行   
        printf("\n");
    }
}

void func(int i, int j)
{
    if (i > j)
        return;
    printf("%dx%d=%d\t", i, j, i * j);
    func(i + 1, j);
}

void f(int n)
{
    if (n == 1)
        printf("1x1=1\n");
    else
    {
        f(n - 1);
        func(1, n);
        putchar('\n');
    }
}


int main()
{
    f(9);
    return 0;
}