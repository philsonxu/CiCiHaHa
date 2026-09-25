#include <stdio.h>

/*
* C 语言经典100例-22
* https://www.runoob.com/cprogramming/c-exercise-example22.html
* 题目：乒乓球队比赛抽签问题
* 两个乒乓球队进行比赛，各出三人，甲队为 a、b、c 三人，乙队为 x、y、z 三人。
* 抽签决定比赛名单，有人向队员打听比赛的名单：a 说他不和 x 比，c 说他不和 x、z 比，请编写代码找出三队赛手的名单。
*/
int main()
{
    printf("乒乓球队比赛抽签问题\n");
    // 定义甲队和乙队的成员
    char teamA[] = { 'a', 'b', 'c' }; // 甲队：a, b, c
    char teamB[] = { 'x', 'y', 'z' }; // 乙队：x, y, z

    // 变量 i, j, k 分别表示 a, b, c 的对手
    char i, j, k;

    // 枚举 a 的对手
    for (i = 'x'; i <= 'z'; i++) {
        // 枚举 b 的对手
        for (j = 'x'; j <= 'z'; j++) {
            // 确保 a 和 b 的对手不同
            if (i != j) {
                // 枚举 c 的对手
                for (k = 'x'; k <= 'z'; k++) {
                    // 确保 c 的对手与 a 和 b 的对手不同
                    if (i != k && j != k) {
                        // 满足题目条件：a 不和 x 比，c 不和 x 和 z 比
                        if (i != 'x' && k != 'x' && k != 'z') {
                            // 输出匹配结果
                            printf("比赛顺序：a--%c\tb--%c\tc--%c\n", i, j, k);
                        }
                    }
                }
            }
        }
    }

    return 0;
}