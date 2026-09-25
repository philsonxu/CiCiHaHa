#include <conio.h>
#include <stdio.h>
#include "graphics.h"

// 定义复数结构体
typedef struct Complex {
    double real;
    double imag;
} Complex;

// 复数乘法
Complex complex_multiply(Complex a, Complex b) {
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.imag * b.real + a.real * b.imag;
    return result;
}

// 复数加法
Complex complex_add(Complex a, Complex b) {
    Complex result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;
    return result;
}

// 计算复数的模
double complex_magnitude(Complex c) {
    return c.real * c.real + c.imag * c.imag;
}

//---------------------------------------------------------------------------
double Hue2RGB(double v1, double v2, double vH)
{
    if (vH < 0) vH += 1;
    if (vH > 1) vH -= 1;
    if (6.0 * vH < 1) return v1 + (v2 - v1) * 6.0 * vH;
    if (2.0 * vH < 1) return v2;
    if (3.0 * vH < 2) return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
    return (v1);
}

int HSLtoRGB(double H, double S, double L)
{
    double R, G, B;
    double var_1, var_2;
    if (S == 0)                       //HSL values = 0 ÷ 1
    {
        R = L * 255.0;                   //RGB results = 0 ÷ 255
        G = L * 255.0;
        B = L * 255.0;
    }
    else
    {
        if (L < 0.5) var_2 = L * (1 + S);
        else         var_2 = (L + S) - (S * L);

        var_1 = 2.0 * L - var_2;

        R = 255.0 * Hue2RGB(var_1, var_2, H + (1.0 / 3.0));
        G = 255.0 * Hue2RGB(var_1, var_2, H);
        B = 255.0 * Hue2RGB(var_1, var_2, H - (1.0 / 3.0));
    }
    //return TColor(RGB(R, G, B));
    long argb = 0;// (int)A << 24;
    argb += (int)R << 0;
    argb += (int)G << 8;
    argb += (int)B << 16;
    return argb;
}


void draw_mandelbrot()
{
    int width = 640;   // 窗口宽度
    int height = 480;  // 窗口高度

    // 复平面查看范围
    double real_min = -2.0;
    double real_max = 1.0;
    double imag_min = -1.5;
    double imag_max = 1.5;

    int max_iterations = 255;  // 最大迭代次数

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // 将像素坐标映射到复平面
            Complex c;
            c.real = real_min + (double)x / width * (real_max - real_min);
            c.imag = imag_min + (double)y / height * (imag_max - imag_min);

            Complex z = { 0, 0 };  // 从z0 = 0开始迭代
            int iteration = 0;

            // 迭代计算
            while (complex_magnitude(z) <= 4.0 && iteration < max_iterations) {
                z = complex_add(complex_multiply(z, z), c);
                iteration++;
            }

            // 设置像素颜色
            if (iteration == max_iterations) {
                putpixel(x, y, BLACK);  // 属于集合的点设为黑色
            }
            else {
                // 使用迭代次数生成颜色
                int color = HSLtoRGB((iteration * 10) % 360, 1.0, 0.5);
                putpixel(x, y, color);
            }
        }
    }
}

/*
* C语言与EasyX实战：手把手实现曼德博分形可视化
* https://blog.csdn.net/m5n6o7/article/details/156004420
*/
int main()
{
    // 初始化图形窗口
    int gdriver = VGA, gmode = VGAHI;
    initgraph(&gdriver, &gmode, "");// "c:\\tc20\\bgi");

    // 绘制曼德博分形
    draw_mandelbrot();

    // 显示提示信息
    //setcolor(WHITE);
    //settextstyle(14, 0, _T("宋体"));
    //outtextxy(10, 10, _T("按任意键退出"));

    // 等待用户按键
    //getch();

    // 关闭图形窗口
    closegraph();

    return 0;
}