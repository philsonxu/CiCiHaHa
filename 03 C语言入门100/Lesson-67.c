#include <stdio.h>
/*
* C 语言实例 - 输出当前文件执行代码
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    FILE* fp;
    char c;
    fp = fopen(__FILE__, "r");
    do {
        c = getc(fp);
        putchar(c);
    } while (c != EOF);
    fclose(fp);
    return 0;
}