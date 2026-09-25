#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 删除字符串中指定字母函数
char* delete_characters(char* str, char* charSet) 
{
    // 用于存储要删除的字符的哈希表
    int hash[256] = { 0 }; // 使用256大小的数组来存储ASCII字符，初始化为0
    // 如果要删除的字符集为空，则直接返回原字符串
    if (charSet == NULL)
        return str;

    // 标记要删除的字符
    for (int i = 0; i < strlen(charSet); i++)
        hash[(unsigned char)charSet[i]] = 1; // 将要删除的字符的ASCII码位置标记为1

    int currentIndex = 0; // 用于追踪当前的字符串索引
    // 通过遍历字符串来删除指定的字符
    for (int i = 0; i < strlen(str); i++) {
        // 如果当前字符不在要删除的字符集中，则保留该字符
        if (!hash[(unsigned char)str[i]])
            str[currentIndex++] = str[i];
    }
    str[currentIndex] = '\0'; // 在字符串的末尾添加空字符，以表示字符串的结束
    return str;
}

/*
* C 语言经典100例-32
* https://www.runoob.com/cprogramming/c-exercise-example32.html
* 题目：删除一个字符串中的指定字母
* 如：字符串 "aca"，删除其中的 a 字母。
*/
int main()
{
    char s[] = "a";      // 要删除的字母
    char s2[] = "aca";   // 目标字符串
    printf("删除一个字符串中的指定字母\n");
    printf("目标字符串:%s\n", s2);
    printf("删除的字母:%s\n", s);
    printf("删除后结果:%s\n", delete_characters(s2, s)); // 打印删除指定字符后的字符串
    return 0;
}