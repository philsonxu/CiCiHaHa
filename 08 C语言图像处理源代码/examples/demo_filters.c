/**
 * demo_filters.c —— 示例2：滤波效果演示
 *
 * 流程：
 *   1. 生成测试图案
 *   2. 分别应用：方框滤波、高斯滤波、中值滤波、拉普拉斯锐化
 *   3. 全部输出BMP对比
 */
#include "imageproc.h"
#include <stdio.h>

int main(void) {
    printf("===== demo_filters: 滤波效果演示 =====\n");
    Image *src = NULL;
    ip_create_test_pattern(400, 300, &src);
    ip_bmp_save("output/filter_original.bmp", src);
    printf("原图 -> output/filter_original.bmp\n");

    /* 方框滤波 5x5 */
    Image *box = NULL;
    ip_blur_box(src, 5, &box);
    ip_bmp_save("output/filter_box5.bmp", box);
    printf("方框5x5 -> output/filter_box5.bmp\n");
    ip_image_release(&box);

    /* 高斯滤波 5x5 σ=1.4 */
    Image *gauss = NULL;
    ip_blur_gaussian(src, 5, 1.4, &gauss);
    ip_bmp_save("output/filter_gauss5.bmp", gauss);
    printf("高斯5x5 σ=1.4 -> output/filter_gauss5.bmp\n");
    ip_image_release(&gauss);

    /* 高斯滤波 9x9 σ=3.0 */
    Image *gauss9 = NULL;
    ip_blur_gaussian(src, 9, 3.0, &gauss9);
    ip_bmp_save("output/filter_gauss9.bmp", gauss9);
    printf("高斯9x9 σ=3.0 -> output/filter_gauss9.bmp\n");
    ip_image_release(&gauss9);

    /* 中值滤波3x3（对椒盐噪声效果最好，见demo_noise_denoise） */
    Image *med = NULL;
    ip_filter_median(src, 3, &med);
    ip_bmp_save("output/filter_median3.bmp", med);
    printf("中值3x3 -> output/filter_median3.bmp\n");
    ip_image_release(&med);

    /* 拉普拉斯锐化 */
    Image *sharp = NULL;
    ip_sharpen_laplacian(src, &sharp);
    ip_bmp_save("output/filter_sharpen.bmp", sharp);
    printf("拉普拉斯锐化 -> output/filter_sharpen.bmp\n");
    ip_image_release(&sharp);

    /* 自定义3x3卷积：浮雕效果 */
    double emboss[3][3] = {{-2,-1,0},{-1,1,1},{0,1,2}};
    Image *em = NULL;
    ip_conv3x3(src, &em, emboss, 1.0, 128);
    ip_bmp_save("output/filter_emboss.bmp", em);
    printf("浮雕效果(自定义卷积核) -> output/filter_emboss.bmp\n");
    ip_image_release(&em);

    ip_image_release(&src);
    printf("===== demo_filters 完成 =====\n\n");
    return 0;
}
