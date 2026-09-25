/*
 * 例10.2 fputc/fgetc 字符读写文件
 * 知识点：逐个字符写入文件、逐个字符从文件读取
 */
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp;
    char ch;
    
    /* 1. 写入文件 */
    fp = fopen("char_test.txt", "w");
    if (fp == NULL) {
        printf("打开文件失败！\n");
        exit(1);
    }
    
    printf("请输入一段文本（以#结束）：\n");
    ch = getchar();
    while (ch != '#') {
        fputc(ch, fp);  /* 将字符写入文件 */
        ch = getchar();
    }
    fclose(fp);
    
    /* 清空输入缓冲区 */
    while (getchar() != '\n');
    
    /* 2. 读取文件并显示 */
    fp = fopen("char_test.txt", "r");
    printf("\n文件内容：\n");
    ch = fgetc(fp);
    while (ch != EOF) {  /* EOF表示文件结束 */
        putchar(ch);
        ch = fgetc(fp);
    }
    fclose(fp);
    printf("\n");
    
    return 0;
}