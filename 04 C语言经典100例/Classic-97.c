#include <stdio.h>
#include <stdlib.h>

/*
* C 语言经典100例-97
* https://www.runoob.com/cprogramming/c-exercise-example97.html
* 题目：键盘输入保存到磁盘文件
* 从键盘输入一些字符，逐个把它们送到磁盘上去，直到输入一个#为止。
*/
int main(void)
{
    FILE* fp = NULL;
    char filename[25];
    char ch;
    printf("输入你要保存到的文件的名称：\n");
    gets(filename);
    if ((fp = fopen(filename, "w")) == NULL)
    {
        printf("error: cannot open file!\n");
        exit(0);
    }
    printf("现在你可以输入你要保存的一些字符，以#结束：\n");
    getchar();
    while ((ch = getchar()) != '#') {
        fputc(ch, fp);
    }
    fclose(fp);
    //system("pause");
    return 0;
}