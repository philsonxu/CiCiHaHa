/*
 * 例10.3 fputs/fgets 字符串读写
 * 知识点：按行读写文本文件
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE *fp;
    char str[100];
    
    /* 写入多行字符串 */
    fp = fopen("string_test.txt", "w");
    if (fp == NULL) {
        printf("打开文件失败！\n");
        exit(1);
    }
    
    printf("请输入3行文本：\n");
    for (int i = 0; i < 3; i++) {
        gets(str);
        fputs(str, fp);
        fputc('\n', fp);  /* 手动加换行 */
    }
    fclose(fp);
    
    /* 读取并显示 */
    printf("\n从文件读取：\n");
    fp = fopen("string_test.txt", "r");
    while (fgets(str, 100, fp) != NULL) {
        printf("%s", str);  /* fgets会读取换行符 */
    }
    fclose(fp);
    
    return 0;
}