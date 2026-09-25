#include <stdio.h>

/*
* C 语言实例 - 判断闰年
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int year;

    printf("输入年份: ");
    scanf("%d", &year);

    if (year % 4 == 0)
    {
        if (year % 100 == 0)
        {
            // 这里如果被 400 整数是闰年
            if (year % 400 == 0)
                printf("%d 是闰年", year);
            else
                printf("%d 不是闰年", year);
        }
        else
        {
            printf("%d 是闰年", year);
        }
    }
    else
    {
        printf("%d 不是闰年", year);
    }

    return 0;
}