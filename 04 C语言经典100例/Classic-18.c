#include <stdio.h>

/*
* C 语言经典100例-18
* https://www.runoob.com/cprogramming/c-exercise-example18.html
* 题目：求 s=a+aa+aaa+aaaa+aa...a 的值，其中 a 是一个数字，
* 例如 2+22+222+2222+22222 (此时共有5个数相加)，几个数相加有键盘控制。
*/
int main()
{
    int sum = 0;        // 存储结果的变量
    int base, terms;    // base 为 a 的值，terms 为 n 的值
    int temp;           // 临时变量，用于计算当前项的值

    // 提示用户输入值
    printf("请输入整数 a 和 n（例如 2 和 5）：");
    scanf("%d %d", &base, &terms);

    // 初始化 temp 为 a
    temp = base;

    // 通过循环计算 a + aa + aaa + ...
    while (terms > 0) {

        printf("%ld", temp);
        if (terms>1) printf(" + ");

        sum += temp;          // 累加当前项
        base *= 10;           // base 每次左移一位（例如 2 -> 20 -> 200）
        temp += base;         // 更新 temp，使其变为下一项
        terms--;              // 减少剩余项数
    }

    // 输出结果
    printf(" = %d\n", sum);
    return 0;
}

// 递归函数，用于计算每一项的值
int calculate_term(int count, int base);

int main()
{
    int count;  // 总项数
    int base;   // 初始值
    int sum = 0; // 累加结果
    int temp = 0;

    // 提示用户输入初始值和总项数
    printf("请输入整数 a 和 n（例如 2 和 5）：");
    scanf("%d %d", &base, &count);

    // 校验输入的合法性
    if (count <= 0 || base <= 0) {
        printf("输入的初始值和项数必须为正整数！\n");
        return 1; // 返回错误状态
    }

    // 通过循环累加每一项
    for (int i = 1; i <= count; i++) {
        temp = calculate_term(i, base);

        printf("%ld", temp);
        if (i < count) printf(" + ");

        sum += temp; // 调用递归函数计算第 i 项的值
    }

    // 输出最终结果
    printf(" = %d\n", sum);
    return 0;
}

// 递归函数，计算当前项的值
int calculate_term(int count, int base) 
{
    if (count == 1) {
        return base; // 递归终止条件：第一项直接返回 base
    }
    else {
        // 递归计算：当前项等于上一项乘以 10 加上 base
        return calculate_term(count - 1, base) * 10 + base;
    }
}