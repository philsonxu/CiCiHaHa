/*
 * 例10.6 文件位置指针操作：rewind、fseek、ftell
 * 知识点：随机读写文件
 * rewind(fp) 将位置指针返回文件开头
 * fseek(fp, 偏移量, 起始点) 移动位置指针
 *   起始点：SEEK_SET(0文件开头) SEEK_CUR(1当前位置) SEEK_END(2文件末尾)
 * ftell(fp) 获取当前位置
 */
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp;
    char str[100];
    
    fp = fopen("position_test.txt", "w+");
    if (fp == NULL) {
        printf("打开文件失败！\n");
        exit(1);
    }
    
    /* 写入数据 */
    fputs("ABCDEFGHIJKLMNOPQRSTUVWXYZ", fp);
    
    /* 用ftell查看当前位置 */
    printf("写入后当前位置：%ld\n", ftell(fp));
    
    /* rewind回到开头 */
    rewind(fp);
    printf("rewind后当前位置：%ld\n", ftell(fp));
    
    /* 读前5个字符 */
    fread(str, 1, 5, fp);
    str[5] = '\0';
    printf("前5个字符：%s\n", str);
    printf("当前位置：%ld\n", ftell(fp));
    
    /* fseek从当前位置向后跳5个字符 */
    fseek(fp, 5, SEEK_CUR);
    fread(str, 1, 5, fp);
    str[5] = '\0';
    printf("跳过5个后读5个：%s\n", str);
    
    /* fseek从开头偏移10 */
    fseek(fp, 10, SEEK_SET);
    fread(str, 1, 5, fp);
    str[5] = '\0';
    printf("从开头偏移10后读5个：%s\n", str);
    
    fclose(fp);
    return 0;
}