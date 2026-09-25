#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* C 语言经典100例-99
* https://www.runoob.com/cprogramming/c-exercise-example99.html
* 题目：文件合并
* 有两个磁盘文件A和B,各存放一行字母，要求把这两个文件中的信息合并（按字母顺序排列），输出到一个新文件C中。
*/
int main(void)
{
    FILE* fa, * fb, * fc;
    int i, j, k;
    char str[100], str1[100];
    char tem;
    if ((fa = fopen("A.txt", "r")) == NULL) // A.txt 文件需要存在
    {
        printf("error: cannot open A file!\n");
        exit(0);
    }
    fgets(str, 99, fa);
    fclose(fa);
    if ((fb = fopen("B.txt", "r")) == NULL)  // B.txt 文件需要存在
    {
        printf("error: cannot open B file!\n");
        exit(0);
    }
    fgets(str1, 100, fb);
    fclose(fb);
    strcat(str, str1);
    for (i = strlen(str) - 1; i > 1; i--)
        for (j = 0; j < i; j++)
            if (str[j] > str[j + 1])
            {
                tem = str[j];
                str[j] = str[j + 1];
                str[j + 1] = tem;
            }

    if ((fc = fopen("C.txt", "w")) == NULL)  // 合并为 C.txt
    {
        printf("error: cannot open C file!\n");
        exit(0);
    }
    fputs(str, fc);
    fclose(fc);
    //system("pause");
    return 0;
}