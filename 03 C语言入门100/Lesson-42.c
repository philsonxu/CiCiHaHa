#include <stdio.h>

/*
* C 语言实例 - 计算数组元素平均值
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    int sum, loop;
    float avg;

    sum = avg = 0;

    for (loop = 0; loop < 10; loop++) {
        sum = sum + array[loop];
    }

    avg = (float)sum / loop;

    printf("平均值为 %.2f", avg);

    return 0;
}

/*======================================================================*/

int main()
{
    int n, i;
    float num[100], sum = 0.0, average;

    printf("输入元素个数: ");
    scanf("%d", &n);

    while (n > 100 || n <= 0)
    {
        printf("Error! 数字需要在1 到 100 之间。\n");
        printf("再次输入: ");
        scanf("%d", &n);
    }

    for (i = 0; i < n; ++i)
    {
        printf("%d. 输入数字: ", i + 1);
        scanf("%f", &num[i]);
        sum += num[i];
    }

    average = sum / n;
    printf("平均值 = %.2f", average);

    return 0;
}

/*======================================================================*/

int main(void)
{
    float x;
    printf("请输入数字：（输入q退出）");
    int i = 0;
    float status;
    float sum = 0;
    float avg = 0;
    status = scanf("%f", &x);
    while (status == 1) {
        sum += x;
        printf("请输入数字：（输入q退出）");
        status = scanf("%f", &x);
        i++;
    }
    avg = sum / (i);
    printf("%.2f", avg);
    return 0;
}

/*======================================================================*/

#define SAFE_FREE(p) \
    free(p);         \
    p = NULL;

int *reverse_sentence();
int leng;

int main()
{
    int* p = NULL;
    printf("输入一组整数: ");
    p = reverse_sentence();

    int sum = 0;
    for (int i = 0; i < leng; i++)
    {
        sum += p[i];
    }
    printf("数组和：%d", sum);

    SAFE_FREE(p);

    return 0;
}

int* reverse_sentence()
{
    static int sum, num;
    static int* p;
    int n;
    char c;
    scanf("%d%c", &n, &c);

    sum++;
    if (c != '\n')
    {
        reverseSentence();
    }
    else
    {
        p = malloc(sizeof(int) * sum);
    }
    num++;
    p[sum - num] = n;

    if (sum == num)
    {
        leng = sum;
        printf("整数数组长度：%d\n", leng);
        sum = num = 0;
    }

    return p;
}