/*
 * 例8.7 指针数组
 * 知识点：数组元素都是指针，适合表示多个字符串
 */
#include <stdio.h>
#include <string.h>

int main()
{
    /* 指针数组：每个元素指向一个字符串 */
    char *weeks[] = {"Monday", "Tuesday", "Wednesday", 
                     "Thursday", "Friday", "Saturday", "Sunday"};
    int i;
    
    for (i = 0; i < 7; i++) {
        printf("星期%d：%s\n", i + 1, weeks[i]);
    }
    
    /* 用指针数组对字符串排序（按字典序）*/
    int n = 5;
    char *names[] = {"Tom", "Jerry", "Alice", "Bob", "Mike"};
    char *temp;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                temp = names[i];
                names[i] = names[j];
                names[j] = temp;
            }
        }
    }
    printf("\n排序后：\n");
    for (i = 0; i < n; i++) {
        printf("%s\n", names[i]);
    }
    
    return 0;
}