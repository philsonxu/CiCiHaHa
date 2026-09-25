/**
 * demo_noise_denoise.c —— 示例5：噪声添加与去噪效果对比（含PSNR定量指标）
 *
 * 对比：
 *   - 高斯噪声  vs 方框/高斯/中值滤波
 *   - 椒盐噪声  vs 方框/高斯/中值滤波（中值滤波对椒盐噪声碾压式优势）
 */
#include "imageproc.h"
#include <stdio.h>

static void save_gray_bmp(const char *path, const Image *g) {
    Image *rgb; ip_gray_to_rgb(g, &rgb);
    ip_bmp_save(path, rgb);
    ip_image_release(&rgb);
}
static void add_gauss_denoise(Image *gray, const char *tag) {
    printf("--- 高斯噪声场景 [%s] ---\n", tag);
    Image *noisy; ip_image_clone(gray, &noisy);
    ip_noise_gaussian(noisy, 25.0);
    save_gray_bmp("output/denoise_gauss_noisy.bmp", noisy);
    double mse,psnr; ip_mse_psnr(gray,noisy,&mse,&psnr);
    printf("  噪声图       PSNR=%.2f dB\n", psnr);

    Image *box; ip_blur_box(noisy,3,&box);
    ip_mse_psnr(gray,box,&mse,&psnr);
    save_gray_bmp("output/denoise_gauss_box3.bmp", box);
    printf("  方框3x3      PSNR=%.2f dB\n", psnr); ip_image_release(&box);

    Image *gauss; ip_blur_gaussian(noisy,5,1.4,&gauss);
    ip_mse_psnr(gray,gauss,&mse,&psnr);
    save_gray_bmp("output/denoise_gauss_gauss5.bmp", gauss);
    printf("  高斯5x5σ1.4  PSNR=%.2f dB\n", psnr); ip_image_release(&gauss);

    Image *med; ip_filter_median(noisy,3,&med);
    ip_mse_psnr(gray,med,&mse,&psnr);
    save_gray_bmp("output/denoise_gauss_median3.bmp", med);
    printf("  中值3x3      PSNR=%.2f dB\n", psnr); ip_image_release(&med);
    ip_image_release(&noisy);
}
static void add_sp_denoise(Image *gray) {
    printf("--- 椒盐噪声场景 ---\n");
    Image *noisy; ip_image_clone(gray, &noisy);
    ip_noise_salt_pepper(noisy, 0.05);
    save_gray_bmp("output/denoise_sp_noisy.bmp", noisy);
    double mse,psnr; ip_mse_psnr(gray,noisy,&mse,&psnr);
    printf("  噪声图(5%%)   PSNR=%.2f dB\n", psnr);

    Image *box; ip_blur_box(noisy,3,&box);
    ip_mse_psnr(gray,box,&mse,&psnr);
    save_gray_bmp("output/denoise_sp_box3.bmp", box);
    printf("  方框3x3      PSNR=%.2f dB\n", psnr); ip_image_release(&box);

    Image *gauss; ip_blur_gaussian(noisy,5,1.4,&gauss);
    ip_mse_psnr(gray,gauss,&mse,&psnr);
    save_gray_bmp("output/denoise_sp_gauss5.bmp", gauss);
    printf("  高斯5x5σ1.4  PSNR=%.2f dB\n", psnr); ip_image_release(&gauss);

    Image *med; ip_filter_median(noisy,3,&med);
    ip_mse_psnr(gray,med,&mse,&psnr);
    save_gray_bmp("output/denoise_sp_median3.bmp", med);
    printf("  中值3x3      PSNR=%.2f dB  ★椒盐噪声最优\n", psnr); ip_image_release(&med);
    ip_image_release(&noisy);
}

int main(void) {
    printf("===== demo_noise_denoise: 噪声与去噪定量对比 =====\n");
    Image *rgb; ip_create_test_pattern(400,300,&rgb);
    Image *gray; ip_rgb_to_gray(rgb, &gray);
    ip_image_release(&rgb);

    add_gauss_denoise(gray, "σ=25");
    add_sp_denoise(gray);

    ip_image_release(&gray);
    printf("===== demo_noise_denoise 完成 =====\n\n");
    return 0;
}
