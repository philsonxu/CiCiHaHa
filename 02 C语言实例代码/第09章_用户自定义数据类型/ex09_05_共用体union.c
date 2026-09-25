/*
 * 例9.5 共用体（联合体）union
 * 知识点：所有成员共用同一段内存，同一时刻只能存一个成员的值
 * 共用体大小 = 最大成员的大小
 */
#include <stdio.h>

union Data {
    int i;
    float f;
    char str[20];
};

int main()
{
    union Data data;
    printf("共用体大小：%lu 字节\n", sizeof(data));
    
    data.i = 10;
    printf("data.i = %d\n", data.i);
    
    data.f = 3.14f;
    printf("data.f = %.2f\n", data.f);
    printf("此时data.i = %d（被覆盖，已非原值）\n", data.i);
    
    /* 典型应用：判断系统大小端 */
    union Endian {
        int num;
        char byte;
    } e;
    e.num = 1;
    if (e.byte == 1)
        printf("当前系统是小端模式(Little Endian)\n");
    else
        printf("当前系统是大端模式(Big Endian)\n");
    
    return 0;
}