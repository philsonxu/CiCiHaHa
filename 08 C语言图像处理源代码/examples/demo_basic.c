/**
 * demo_basic.c —— 示例1：基础IO、图像创建、亮度/对比度/反色、格式转换
 *
 * 演示流程：
 *   1. 生成一张测试图案（渐变+矩形+圆+线）
 *   2. 存为BMP/PPM
 *   3. 加亮度、调对比度、反色
 *   4. 转灰度（NTSC加权）、存为PGM
 *   5. 再加载回来验证
 */
#include "imageproc.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("===== demo_basic: 基础IO与像素调整 =====\n");
    /* 1) 生成测试图案 */
    Image *src = NULL;
    IpStatus s = ip_create_test_pattern(400, 300, &src);
    printf("ip_create_test_pattern(400x300): %s\n", ip_status_str(s));
    ip_image_info(src);

    /* 2) 存BMP */
    s = ip_bmp_save("output/test_pattern.bmp", src);
    printf("ip_bmp_save: %s -> output/test_pattern.bmp\n", ip_status_str(s));
    s = ip_ppm_save("output/test_pattern.ppm", src, 1);
    printf("ip_ppm_save(P6): %s -> output/test_pattern.ppm\n", ip_status_str(s));

    /* 3) 亮度+50 */
    Image *bright = NULL; ip_image_clone(src, &bright);
    ip_brightness(bright, 50);
    ip_bmp_save("output/brightness_up.bmp", bright);
    printf("亮度+50 -> output/brightness_up.bmp\n");

    /* 4) 对比度1.5 */
    Image *cont = NULL; ip_image_clone(src, &cont);
    ip_contrast(cont, 1.5);
    ip_bmp_save("output/contrast_1.5.bmp", cont);
    printf("对比度x1.5 -> output/contrast_1.5.bmp\n");

    /* 5) 反色 */
    Image *inv = NULL; ip_image_clone(src, &inv);
    ip_invert(inv);
    ip_bmp_save("output/invert.bmp", inv);
    printf("反色 -> output/invert.bmp\n");

    /* 6) 转灰度（NTSC加权） */
    Image *gray = NULL;
    ip_rgb_to_gray(src, &gray);
    ip_pgm_save("output/gray.pgm", gray, 1);
    ip_bmp_save("output/gray.bmp", gray); /* 灰度转BMP需先转RGB；这里单独演示pgm */
    printf("灰度(NTSC) -> output/gray.pgm\n");

    /* 7) 灰度图转RGB再存BMP */
    Image *gray_rgb = NULL;
    ip_gray_to_rgb(gray, &gray_rgb);
    ip_bmp_save("output/gray_as_bmp.bmp", gray_rgb);
    printf("灰度→BMP -> output/gray_as_bmp.bmp\n");

    /* 8) 读取刚保存的BMP验证 */
    Image *reload = NULL;
    ip_bmp_load("output/test_pattern.bmp", &reload);
    printf("重新加载BMP: %s, ", ip_status_str(s));
    ip_image_info(reload);
    double mse=0, psnr=0;
    ip_mse_psnr(src, reload, &mse, &psnr);
    printf("加载回传MSE=%.2f, PSNR=%.2f dB\n", mse, psnr);

    ip_image_release(&src); ip_image_release(&bright); ip_image_release(&cont);
    ip_image_release(&inv); ip_image_release(&gray); ip_image_release(&gray_rgb);
    ip_image_release(&reload);
    printf("===== demo_basic 完成 =====\n\n");
    return 0;
}
