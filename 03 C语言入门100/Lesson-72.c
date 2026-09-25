#include <stdio.h>

/*
* C 语言实例 - 数字二进制文件读写
* https://www.runoob.com/cprogramming/c-examples.html
*/

int main()
{
    // 原始数据数组
    int data[] = { 1, 2, 3, 4, 5 };
    int count = 5;  // 数组元素个数

    // ===== 写入数组到文件 =====
    FILE* fp = fopen("array.bin", "wb");
    if (fp == NULL) {
        perror("文件打开失败");
        return 1;
    }
    // data 等价于 &data[0]，指向数组首地址
    fwrite(data, sizeof(int), count, fp);
    fclose(fp);
    printf("已写入 %d 个整数到 array.bin\n", count);

    // ===== 从文件读取数组 =====
    int buffer[5] = { 0 };  // 初始化为 0，便于验证读取结果
    fp = fopen("array.bin", "rb");
    if (fp == NULL) {
        perror("文件打开失败");
        return 1;
    }
    size_t read_count = fread(buffer, sizeof(int), count, fp);
    if (read_count < count) {
        printf("读取不完整：预期 %d 个，实际 %zu 个\n", count, read_count);
    }
    fclose(fp);

    // 打印读取结果
    printf("读取到的数据: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");
    return 0;
}