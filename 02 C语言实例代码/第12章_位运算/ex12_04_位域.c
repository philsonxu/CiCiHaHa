/*
 * 例12.4 位域（位段）
 * 知识点：结构体中成员按位存储，节省存储空间
 * 用于硬件寄存器、网络协议等需要精确控制位宽的场景
 */
#include <stdio.h>

/* 定义一个用位域表示的日期/时间结构体 */
struct PackedDate {
    unsigned int day   : 5;   /* 日 0-31，5位足够 */
    unsigned int month : 4;   /* 月 0-12，4位足够 */
    unsigned int year  : 12;  /* 年 0-4095，12位足够 */
    unsigned int       : 11;  /* 预留位 */
};

/* 结构体总大小：5+4+12+11=32位 = 4字节，比用4个int(16字节)省很多 */

int main()
{
    struct PackedDate d;
    d.day = 23;
    d.month = 9;
    d.year = 2024;
    
    printf("日期：%u年%u月%u日\n", d.year, d.month, d.day);
    printf("位域结构体大小：%lu 字节\n", sizeof(struct PackedDate));
    
    return 0;
}