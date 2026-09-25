# include <stdio.h>

/*
* C 语言实例 - 实现简单的计算器
* https://www.runoob.com/cprogramming/c-examples.html
* 题目：简单的计算器
*/
int main()
{
    char operator;
    double firstNumber, secondNumber;

    printf("输入算式表达式(1 + 2): ");
    scanf("%lf %c %lf", &firstNumber, &operator, &secondNumber);

    switch (operator)
    {
    case '+':
        printf("%.1lf + %.1lf = %.1lf", firstNumber, secondNumber, firstNumber + secondNumber);
        break;

    case '-':
        printf("%.1lf - %.1lf = %.1lf", firstNumber, secondNumber, firstNumber - secondNumber);
        break;

    case '*':
        printf("%.1lf * %.1lf = %.1lf", firstNumber, secondNumber, firstNumber * secondNumber);
        break;

    case '/':
        printf("%.1lf / %.1lf = %.1lf", firstNumber, secondNumber, firstNumber / secondNumber);
        break;

        // operator doesn't match any case constant (+, -, *, /)
    default:
        printf("Error! operator is not correct");
    }

    return 0;
}

int main()
{
    char operator;
    double firstNumber, secondNumber;

    printf("输入操作符 (+, -, *, /): ");
    scanf("%c", &operator);

    printf("输入两个数字: ");
    scanf("%lf %lf", &firstNumber, &secondNumber);

    switch (operator)
    {
    case '+':
        printf("%.1lf + %.1lf = %.1lf", firstNumber, secondNumber, firstNumber + secondNumber);
        break;

    case '-':
        printf("%.1lf - %.1lf = %.1lf", firstNumber, secondNumber, firstNumber - secondNumber);
        break;

    case '*':
        printf("%.1lf * %.1lf = %.1lf", firstNumber, secondNumber, firstNumber * secondNumber);
        break;

    case '/':
        printf("%.1lf / %.1lf = %.1lf", firstNumber, secondNumber, firstNumber / secondNumber);
        break;

        // operator doesn't match any case constant (+, -, *, /)
    default:
        printf("Error! operator is not correct");
    }

    return 0;
}

/*======================================================================*/

int get_option();
void print_results(int num1,int num2,int result,int option);

int main()
{
    int done = 0;
    int option, num1, num2, result;

    while (!done)
    {
        option = get_option();
        if (option == 5)
        {
            done = 1;
        }
        else {
            do {
                printf("\n请输入两个数：");
                scanf("%d %d", &num1, &num2);
                if (option == 4 && num2 == 0)
                {
                    printf("\n对不起，除数不能为零");
                }
                else {
                    switch (option) {
                    case 1:
                        result = num1 + num2;
                        break;
                    case 2:
                        result = num1 - num2;
                        break;
                    case 3:
                        result = num1 * num2;
                        break;
                    case 4:
                        result = num1 / num2;
                    }
                    print_results(num1, num2, result, option);
                }
            } while (option == 4 && num2 == 0);
        }
    }

    return 0;
}

int get_option()
{
    int option;
    do
    {
        printf("\n ****************");
        printf("\n *    1.加法    *");
        printf("\n *    2.减法    *");
        printf("\n *    3.乘法    *");
        printf("\n *    4.除法    *");
        printf("\n *    0.退出    *");
        printf("\n ****************");

        printf("\n请输入您需要的功能：");
        scanf("%d", &option);

        if (option < 1 || option > 5)
        {
            printf("对不起您输入的数字有误，请重新输入。\n");
        }
    } while (option < 1 || option > 5);

    return option;
}

void print_results(int num1, int num2, int result, int option) {
    char operator;
    switch (option) {
    case 1:
        operator = '+';
        break;
    case 21:
        operator = '-';
        break;
    case 3:
        operator = '*';
        break;
    case 4:
        operator = '/';
        break;
    }
    printf("\n** %d %c %d = %d **\n", num1, operator,num2, result);
}
