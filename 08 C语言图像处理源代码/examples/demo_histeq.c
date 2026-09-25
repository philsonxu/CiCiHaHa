/**
 * demo_histeq.c —— 示例8：直方图统计与均衡化
 *
 * 演示：
 *   1. 生成一幅偏暗图像
 *   2. 统计直方图并输出ASCII
 *   3. 直方图均衡化
 *   4. 均衡化后再统计直方图对比
 */
#include "imageproc.h"
#include <stdio.h>
#include <math.h>

int main(void) {
    printf("===== demo_histeq: 直方图统计与均衡化 =====\n");
    /* 1) 生成偏暗测试图（中心暗、边缘亮的径向渐变+一点噪声） */
    int W=400, H=300;
    Image *rgb; ip_image_create(W,H,IP_FMT_RGB24,&rgb);
    for (int y=0; y<H; y++) {
        for (int x=0; x<W; x++) {
            double dx=x-W/2.0, dy=y-H/2.0;
            double d = sqrt(dx*dx+dy*dy);
            /* 边缘0中心高 → 整体偏暗 */
            double v = 255 - d/2.8;
            if (v < 30) v = 30;
            uint8_t u = (uint8_t)v;
            IP_R(rgb,x,y)=u; IP_G(rgb,x,y)=u; IP_B(rgb,x,y)=u;
        }
    }
    ip_bmp_save("output/histeq_dark.bmp", rgb);
    printf("偏暗原图 -> output/histeq_dark.bmp\n");

    /* 2) 转灰度 + 统计直方图 */
    Image *gray; ip_rgb_to_gray(rgb, &gray);
    uint64_t hist[256];
    ip_histogram(gray, hist);
    ip_histogram_save_ascii("output/histeq_before.txt", hist, 20);
    printf("均衡前直方图 -> output/histeq_before.txt\n");

    double mean, std;
    ip_mean_stddev(gray, &mean, &std);
    printf("均衡前: mean=%.1f, stddev=%.1f\n", mean, std);

    /* 3) 直方图均衡化 */
    ip_histogram_equalize(gray);
    Image *eq_rgb; ip_gray_to_rgb(gray, &eq_rgb);
    ip_bmp_save("output/histeq_after.bmp", eq_rgb);
    printf("均衡化后 -> output/histeq_after.bmp\n");

    /* 4) 均衡化后直方图 */
    ip_histogram(gray, hist);
    ip_histogram_save_ascii("output/histeq_after.txt", hist, 20);
    printf("均衡后直方图 -> output/histeq_after.txt\n");
    ip_mean_stddev(gray, &mean, &std);
    printf("均衡后: mean=%.1f, stddev=%.1f\n", mean, std);

    ip_image_release(&rgb); ip_image_release(&gray); ip_image_release(&eq_rgb);
    printf("===== demo_histeq 完成 =====\n\n");
    return 0;
}
