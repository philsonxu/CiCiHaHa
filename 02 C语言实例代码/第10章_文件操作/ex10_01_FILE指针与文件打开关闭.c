/*
 * 例10.1 文件的打开与关闭
 * 知识点：FILE文件指针、fopen打开文件、fclose关闭文件
 * 打开模式："r"读 "w"写 "a"追加 "r+"读写 "w+"读写创建 "a+"追加读
 * "b"表示二进制模式，如"rb" "wb"
 */
#include <stdio.h>

int main()
{
    FILE *fp;
    
    /* 以写入模式打开文件 */
    fp = fopen("test.txt", "w");
    if (fp == NULL) {
        printf("无法打开文件！\n");
        return 1;
    }
    printf("文件打开成功！\n");
    
    /* ... 进行文件读写操作 ... */
    
    fclose(fp);  /* 用完后必须关闭文件 */
    printf("文件已关闭。\n");
    return 0;
}