#include <stdio.h>
#include <stdlib.h>

int string_length(char* s);

/*
* C 语言经典100例-70
* https://www.runoob.com/cprogramming/c-exercise-example70.html
* 题目：字符串长度
* 写一个函数，求一个字符串的长度，在 main 函数中输入字符串，并输出其长度。
*/
int main()
{
    char str[100]; // 可以根据实际情况增大数组长度
    printf("字符串长度问题\n");
    printf("请输入字符串:\n");
    scanf("%s", str);

    int len = string_length(str); // 调用 length 函数计算字符串长度

    printf("字符串有 %d 个字符（汉字占2个）。\n", len);
    return EXIT_SUCCESS;
}

//求字符串长度  
int string_length(char* s) 
{
    int i = 0;
    while (*s != '\0') {
        i++;
        s++;
    }
    return i;
}