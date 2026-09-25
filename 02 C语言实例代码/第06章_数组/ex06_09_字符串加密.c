/*
 * 例6.9 字符串加密（经典例题：China加密）
 * 规则：将每个字母替换为它后面第4个字母（如A→E, B→F,...）
 * 知识点：字符数组遍历、字符ASCII运算
 */
#include <stdio.h>
#include <string.h>

int main()
{
    char str[] = "China";
    int i;
    
    printf("原字符串：%s\n", str);
    
    for (i = 0; i < strlen(str); i++) {
        str[i] = str[i] + 4;
    }
    
    printf("加密后：%s\n", str);
    return 0;
}