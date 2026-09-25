#include <stdio.h>
/*
* C 语言实例 - 将字符串写入文件
* https://www.runoob.com/cprogramming/c-examples.html
*/
#include <stdlib.h>  /* exit() 函数 */

int main()
{
    char sentence[1000];
    FILE* fptr;

    fptr = fopen("runoob.txt", "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    printf("输入字符串:\n");
    fgets(sentence, (sizeof sentence / sizeof sentence[0]), stdin);

    fprintf(fptr, "%s", sentence);
    fclose(fptr);

    return 0;
}

int main()
{
    char msg[1000];
    FILE* stream;
    int i = 0;
    if ((stream = fopen("G:/text/abc.txt", "w")) == NULL)
    {
        perror("fail to write");
        exit(1);
    }
    scanf("%s", msg);    //控制台键入内容到文件中
    while (msg[i])
    {
        fputc(msg[i], stream);
        i++;
    }
    return 0;
}


#define MAX_LINE 1024
int main()
{
    char buf[MAX_LINE];  /*缓冲区*/
    FILE* fp;            /*文件指针*/
    int len;             /*行字符个数*/
    if ((fp = fopen("test.txt", "r")) == NULL)
    {
        perror("fail to read");
        exit(1);
    }
    while (fgets(buf, MAX_LINE, fp) != NULL)
    {
        len = strlen(buf);
        buf[len - 1] = '\0';  /*去掉换行符*/
        printf("%s %d \n", buf, len - 1);
    }
    return 0;
}
