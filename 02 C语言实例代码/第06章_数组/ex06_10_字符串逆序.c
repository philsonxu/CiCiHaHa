/*
 * 例6.10 字符串逆序
 * 知识点：字符数组操作、首尾指针/下标交换法
 */
#include <stdio.h>
#include <string.h>

int main()
{
    char str[100];
    int i, len;
    char t;
    
    printf("请输入一个字符串：");
    scanf("%s", str);
    
    len = strlen(str);
    for (i = 0; i < len / 2; i++) {
        t = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = t;
    }
    
    printf("逆序后：%s\n", str);
    return 0;
}