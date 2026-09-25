#include <stdio.h>
#include <math.h>

/*
* C 语言实例 - 一元二次方程
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    float a, b, c, x1, x2, d;
    printf("输入方程的三个系数:");
    scanf("%f %f %f", &a, &b, &c);
    if (a != 0)
    {
        d = sqrt(b * b - 4 * a * c);
        x1 = (-b + d) / (2 * a);
        x2 = (-b - d) / (2 * a);
        if (x1 < x2)
            printf("%0.2f %0.2f\n", x2, x1);
        else
            printf("%0.2f %0.2f\n", x1, x2);
    }
    return 0;
}

int main(void)
{
    double a, b, c;
    double delat;
    double x1, x2;
    char ch;
    do
    {
        printf("请输入一元二次方程的三个系数：\n");

        printf("请输入系数 a = \t");
        scanf("%lf", &a);

        printf("请输入系数 b = \t");
        scanf("%lf", &b);

        printf("请输入系数 c = \t");
        scanf("%lf", &c);

        delat = b * b - 4 * a * c;

        if (delat > 0)
        {
            x1 = (-b + sqrt(delat)) / (2 * a);;
            x2 = (-b - sqrt(delat)) / (2 * a);;
            printf("有2个实数解：x1 = %lf  x2 = %lf\n", x1, x2);
        }
        else if (delat == 0)
        {
            x1 = (-b + sqrt(delat)) / (2 * a);;
            printf("有2个相等实数解：x1=x2 = %lf\n", x1);
        }
        else
            printf("无实数解\n");

        printf("是否继续吗：Y/N\n");
        scanf(" %c", &ch);
    } while (ch == 'y' || ch == 'Y');
    return 0;
}

int main()
{
    double disc, a, b, c, p, q, x1, x2;
    printf("请输入a和b和c的值：\n");
    scanf("%lf %lf %lf", &a, &b, &c);
    disc = b * b - 4 * a * c;
    if (disc < 0)
        printf("改方程无实数根\n");
    else
    {
        p = (-b) / (2 * a);
        q = sqrt(disc) / (2 * a);
        x1 = p + q;
        x2 = p - q;
        printf("x1=%7.2f\nx2=%7.2f\n", x1, x2);
    }
    return 0;
}