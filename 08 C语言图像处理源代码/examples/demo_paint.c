/**
 * demo_paint.c —— 示例7：基础绘图API实战
 *
 * 生成几幅有意义的合成图：
 *   1. 同心圆靶心
 *   2. 棋盘格（相机标定用）
 *   3. 模拟时钟表盘
 *   4. 随机点散布
 */
#include "imageproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void) {
    printf("===== demo_paint: 图元绘制实战 =====\n");
    srand((unsigned)time(NULL));

    /* 1. 同心圆靶心 */
    Image *tgt; ip_image_create(400,400,IP_FMT_RGB24,&tgt);
    /* 白色背景 */
    for (int i=0;i<400*400*3;i++) tgt->data[i]=255;
    for (int r=190; r>=10; r-=20) {
        uint8_t col = (r/20)%2==0 ? 0 : 255;
        ip_draw_circle(tgt, 200, 200, r, col, col, col, 1);
    }
    ip_draw_circle(tgt, 200,200, 10, 255,0,0, 1);
    ip_bmp_save("output/paint_target.bmp", tgt);
    printf("同心圆靶心 -> output/paint_target.bmp\n");
    ip_image_release(&tgt);

    /* 2. 棋盘格 8x8 */
    Image *cb; ip_image_create(400,400,IP_FMT_RGB24,&cb);
    int cell = 400/8;
    for (int gy=0; gy<8; gy++) {
        for (int gx=0; gx<8; gx++) {
            uint8_t c = ((gx+gy)%2==0) ? 0 : 255;
            ip_draw_rect(cb, gx*cell, gy*cell, cell, cell, c,c,c, 1);
        }
    }
    ip_bmp_save("output/paint_chessboard.bmp", cb);
    printf("8x8棋盘格(相机标定用) -> output/paint_chessboard.bmp\n");
    ip_image_release(&cb);

    /* 3. 模拟时钟表盘 */
    int SZ = 400;
    Image *clk; ip_image_create(SZ,SZ,IP_FMT_RGB24,&clk);
    for (int i=0;i<SZ*SZ*3;i++) clk->data[i]=255;
    ip_draw_circle(clk, SZ/2, SZ/2, SZ/2-5, 0,0,0, 0);
    /* 刻度 */
    for (int k=0; k<12; k++) {
        double ang = k*M_PI/6 - M_PI/2;
        int x1 = (int)(SZ/2 + cos(ang)*(SZ/2-20));
        int y1 = (int)(SZ/2 + sin(ang)*(SZ/2-20));
        int x2 = (int)(SZ/2 + cos(ang)*(SZ/2-40));
        int y2 = (int)(SZ/2 + sin(ang)*(SZ/2-40));
        ip_draw_line(clk, x1,y1,x2,y2, 0,0,0);
    }
    /* 时针：指向10点10分 */
    double h_ang = 10*M_PI/6 - M_PI/2 + 10*M_PI/360;
    ip_draw_line(clk, SZ/2, SZ/2,
                 (int)(SZ/2 + cos(h_ang)*100), (int)(SZ/2 + sin(h_ang)*100),
                 0,0,0);
    /* 分针 */
    double m_ang = 10*M_PI/30 - M_PI/2;
    ip_draw_line(clk, SZ/2, SZ/2,
                 (int)(SZ/2 + cos(m_ang)*140), (int)(SZ/2 + sin(m_ang)*140),
                 200,0,0);
    /* 秒针 */
    ip_draw_line(clk, SZ/2, SZ/2,
                 (int)(SZ/2 + cos(-M_PI/2 + 30*M_PI/30)*160),
                 (int)(SZ/2 + sin(-M_PI/2 + 30*M_PI/30)*160),
                 0,0,200);
    ip_draw_circle(clk, SZ/2, SZ/2, 6, 0,0,0,1);
    ip_bmp_save("output/paint_clock.bmp", clk);
    printf("时钟表盘 -> output/paint_clock.bmp\n");
    ip_image_release(&clk);

    /* 4. 随机彩点散布 */
    Image *dp; ip_image_create(500,500,IP_FMT_RGB24,&dp);
    for (int i=0;i<500*500*3;i++) { dp->data[i]=(uint8_t)(i%256); }
    /* 背景渐变 */
    for (int y=0; y<500; y++)
        for (int x=0; x<500; x++) {
            IP_R(dp,x,y) = (uint8_t)(x/2);
            IP_G(dp,x,y) = (uint8_t)(y/2);
            IP_B(dp,x,y) = (uint8_t)(100);
        }
    for (int i=0; i<200; i++) {
        int x = rand()%500, y = rand()%500;
        int r = 3+rand()%12;
        uint8_t cr = rand()%256, cg=rand()%256, cb=rand()%256;
        ip_draw_circle(dp, x,y,r, cr,cg,cb, 1);
    }
    ip_bmp_save("output/paint_dots.bmp", dp);
    printf("200个随机彩色圆点 -> output/paint_dots.bmp\n");
    ip_image_release(&dp);

    printf("===== demo_paint 完成 =====\n\n");
    return 0;
}
