#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 处理是否继续游戏的逻辑
int ask_to_play_again() 
{
    char begin;
    printf("需要挑战最高级别不？Y/N \n");
    scanf(" %c", &begin);  // 注意前面有个空格，确保跳过上次输入的换行符
    if (begin == 'Y' || begin == 'y') {
        return 1;  // 用户选择继续游戏
    }
    printf("谢谢，再见!\n");
    return 0;  // 用户选择退出游戏
}

// 游戏的主体逻辑
void caizi(void)
{
    int n;
    int count = 1;
    srand((unsigned int)time(NULL));  // 更严谨地处理随机数种子
    int m = (rand() % 100) + 1;

    printf("猜数字（1-100）游戏开始，请输入数字:\n");

    while (1) {
        scanf("%d", &n);

        if (n == m) {
            printf("猜中了，使用了 %d 次！\n", count);

            // 根据猜测次数，给出不同的评价
            if (count == 1) {
                printf("你是神级人物了！膜拜\n");
            }
            else if (count <= 5) {
                printf("你是王级人物了！非常赞\n");
            }
            else if (count <= 10) {
                printf("你是大师级人物了！狂赞\n");
            }
            else if (count <= 15) {
                printf("你是钻石级人物了！怒赞\n");
            }
            else {
                printf("你的技术还有待提高哦！\n");
            }

            // 询问是否继续游戏
            if (ask_to_play_again()) {
                caizi();  // 重新开始游戏
            }
            break;
        }
        else if (n < m) {
            puts("太小了!");
            puts("重新输入:");
        }
        else {
            puts("太大了!");
            puts("重新输入:");
        }

        count++;  // 计数器
    }
}

/*
* C 语言经典100例-94
* https://www.runoob.com/cprogramming/c-exercise-example94.html
* 题目：猜谜游戏
*/
int main(void)
{
    caizi();
    return 0;
}