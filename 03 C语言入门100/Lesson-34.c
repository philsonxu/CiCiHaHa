#include <stdio.h>

/*
* C 语言实例 - 创建各类三角形图案
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int i, j, rows;

    printf("行数: ");
    scanf("%d", &rows);

    for (i = 1; i <= rows; ++i)
    {
        for (j = 1; j <= i; ++j)
        {
            printf("* ");
        }
        printf("\n");
    }
    return 0;
}


int main()
{
    int i, j, rows;

    printf("行数: ");
    scanf("%d", &rows);

    for (i = 1; i <= rows; ++i)
    {
        for (j = 1; j <= i; ++j)
        {
            printf("%d ", j);
        }
        printf("\n");
    }
    return 0;
}


int main()
{
    int i, j;
    char input, alphabet = 'A';

    printf("输入大写字母: ");
    scanf("%c", &input);

    for (i = 1; i <= (input - 'A' + 1); ++i)
    {
        for (j = 1; j <= i; ++j)
        {
            printf("%c", alphabet);
        }
        ++alphabet;

        printf("\n");
    }
    return 0;
}


int main()
{
    int i, j, rows;

    printf("行数: ");
    scanf("%d", &rows);

    for (i = rows; i >= 1; --i)
    {
        for (j = 1; j <= i; ++j)
        {
            printf("* ");
        }
        printf("\n");
    }

    return 0;
}


int main()
{
    int i, j, rows;

    printf("行数: ");
    scanf("%d", &rows);

    for (i = rows; i >= 1; --i)
    {
        for (j = 1; j <= i; ++j)
        {
            printf("%d ", j);
        }
        printf("\n");
    }

    return 0;
}


int main()
{
    int i, space, rows, k = 0;

    printf("Enter number of rows: ");
    scanf("%d", &rows);

    for (i = 1; i <= rows; ++i, k = 0)
    {
        for (space = 1; space <= rows - i; ++space)
        {
            printf("  ");
        }

        while (k != 2 * i - 1)
        {
            printf("* ");
            ++k;
        }

        printf("\n");
    }

    return 0;
}


int main()
{
    int i, space, rows, k = 0, count = 0, count1 = 0;

    printf("行数: ");
    scanf("%d", &rows);

    for (i = 1; i <= rows; ++i)
    {
        for (space = 1; space <= rows - i; ++space)
        {
            printf("  ");
            ++count;
        }

        while (k != 2 * i - 1)
        {
            if (count <= rows - 1)
            {
                printf("%d ", i + k);
                ++count;
            }
            else
            {
                ++count1;
                printf("%d ", (i + k - 2 * count1));
            }
            ++k;
        }
        count1 = count = k = 0;

        printf("\n");
    }
    return 0;
}


int main()
{
    int rows, i, j, space;

    printf("行数: ");
    scanf("%d", &rows);

    for (i = rows; i >= 1; --i)
    {
        for (space = 0; space < rows - i; ++space)
            printf("  ");

        for (j = i; j <= 2 * i - 1; ++j)
            printf("* ");

        for (j = 0; j < i - 1; ++j)
            printf("* ");

        printf("\n");
    }

    return 0;
}

// 杨辉三角

int main()
{
    int rows, coef = 1, space, i, j;

    printf("行数: ");
    scanf("%d", &rows);

    for (i = 0; i < rows; i++)
    {
        for (space = 1; space <= rows - i; space++)
            printf("  ");

        for (j = 0; j <= i; j++)
        {
            if (j == 0 || i == 0)
                coef = 1;
            else
                coef = coef * (i - j + 1) / j;

            printf("%4d", coef);
        }
        printf("\n");
    }

    return 0;
}

// 弗洛伊德三角形

#define N 10 
int main()
{
    int i, j, l;
    for (i = 1, j = 1; i <= N; i++)
    {
        for (l = 1; l <= i; l++, j++)
            printf("%5d", j);
        printf("\n");
    }
    return 0;
}


int main()
{
    int i, j, k, n;
    printf("请输入数字金字塔基座宽度(正奇数):");
    scanf("%d", &n);
    for (k = 1; k <= (n + 1) / 2; k++) {
        for (i = 1; i < (n + 1) / 2 + 1 - k; i++)    printf(" ");
        for (j = k; j < (2 * k); j++) printf("%d", j);
        for (j -= 2; j >= k; j--) printf("%d", j);
        printf("\n");
    }
}


int main()
{
    // 变量声明
    int blank, number, column, n;

    // 用户输入金字塔层数
    printf("请输入数字金字塔层数（小于50）:");
    scanf("%d", &n);

    // 判断输出三角形中最大数的位数，即最后一行中间数的位数
    int count = 0;
    int x = 0;
    x = 2 * n - 1;  // 最后一行中间数数值
    while (x != 0) {
        x /= 10;
        ++count;
    }

    // 输出金字塔
    for (column = 1; column <= n; column++) {
        // 输出每行前部空格
        for (blank = 1; blank < (n - column) * (count + 1) + count; blank++)
            printf(" ");

        // 输出从小到大的数字
        for (number = column; number < (2 * column); number++) {
            if (number < 10 && count != 1) {
                printf("  %d", number);
            }
            else {
                printf(" %d", number);
            }
        }

        // 输出从大到小的数字
        for (number -= 2; number >= column; number--) {
            if (number < 10 && count != 1) {
                printf("  %d", number);
            }
            else {
                printf(" %d", number);
            }
        }

        printf("\n\n");
    }
    return 0;
}