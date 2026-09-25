/**
 * test_pipeline.c —— 综合处理流水线验证
 *
 * 模拟一个真实场景：
 *   "一张带噪声的照片 → 去噪 → 转灰度 → 二值化 → 形态学去噪 → 边缘检测"
 * 验证每一步输出尺寸/通道正确、像素值范围合法、关键指标符合预期。
 */
#include "imageproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <sys/stat.h>

int main(void) {
    mkdir("output", 0755);
    printf("===== test_pipeline: 完整图像处理流水线验证 =====\n\n");

    /* Step 0: 创建输入（模拟带噪声照片=测试图案+高斯噪声σ=20） */
    printf("[Step 0] 创建输入+加噪声 ...\n");
    Image *raw=NULL;
    assert(ip_create_test_pattern(320,240,&raw)==IP_OK);
    ip_noise_gaussian(raw, 20.0);
    assert(raw->width==320 && raw->height==240 && raw->channels==3);
    ip_bmp_save("output/pipe_raw.bmp", raw);
    printf("  原始带噪声图 320x240 RGB -> output/pipe_raw.bmp\n");

    /* Step 1: 高斯去噪 */
    printf("[Step 1] 高斯滤波去噪 (5x5 σ=1.4) ...\n");
    Image *denoised=NULL;
    assert(ip_blur_gaussian(raw, 5, 1.4, &denoised)==IP_OK);
    double mse_a,psnr_a; ip_mse_psnr(raw,denoised,&mse_a,&psnr_a);
    assert(denoised->width==320 && denoised->height==240);
    ip_bmp_save("output/pipe_denoised.bmp", denoised);
    printf("  输出 320x240 RGB, 相对原图PSNR=%.2f dB\n", psnr_a);
    ip_image_release(&raw);

    /* Step 2: 转灰度 */
    printf("[Step 2] RGB→Gray(NTSC加权) ...\n");
    Image *gray=NULL;
    assert(ip_rgb_to_gray(denoised, &gray)==IP_OK);
    assert(gray->fmt==IP_FMT_GRAY8 && gray->channels==1);
    {
        Image *gr; ip_gray_to_rgb(gray,&gr);
        ip_bmp_save("output/pipe_gray.bmp", gr);
        ip_image_release(&gr);
    }
    double mean,std;
    ip_mean_stddev(gray, &mean, &std);
    printf("  输出 320x240 Gray, 均值=%.1f, 标准差=%.1f\n", mean, std);
    assert(mean > 30 && mean < 220);
    ip_image_release(&denoised);

    /* Step 3: 直方图均衡化增强对比度 */
    printf("[Step 3] 直方图均衡化 ...\n");
    double mean_b, std_b;
    ip_mean_stddev(gray, &mean_b, &std_b);
    assert(ip_histogram_equalize(gray)==IP_OK);
    ip_mean_stddev(gray, &mean, &std);
    {
        Image *gr; ip_gray_to_rgb(gray,&gr);
        ip_bmp_save("output/pipe_histeq.bmp", gr);
        ip_image_release(&gr);
    }
    printf("  均衡后 stddev: %.1f -> %.1f (应增大)\n", std_b, std);
    assert(std > std_b);

    /* Step 4: Otsu自动二值化 */
    printf("[Step 4] Otsu自动二值化 ...\n");
    uint8_t t; assert(ip_threshold_otsu(gray,&t)==IP_OK);
    assert(ip_threshold(gray, t, 255)==IP_OK);
    {
        Image *gr; ip_gray_to_rgb(gray,&gr);
        ip_bmp_save("output/pipe_binary.bmp", gr);
        ip_image_release(&gr);
    }
    /* 验证二值图只有0和255 */
    int only01=1;
    for (int i=0;i<320*240;i++) if (gray->data[i]!=0 && gray->data[i]!=255) only01=0;
    assert(only01);
    printf("  自动阈值t=%d, 二值图仅含0/255: %s\n", t, only01?"OK":"FAIL");
    assert(t>20 && t<240);

    /* Step 5: 形态学开运算去除噪点 */
    printf("[Step 5] 形态学开运算(3x3)去斑点 ...\n");
    Image *morphed=NULL;
    assert(ip_morph_open(gray, 3, &morphed)==IP_OK);
    {
        Image *gr; ip_gray_to_rgb(morphed,&gr);
        ip_bmp_save("output/pipe_morph_open.bmp", gr);
        ip_image_release(&gr);
    }
    ip_image_release(&gray);
    printf("  输出 320x240 Gray -> output/pipe_morph_open.bmp\n");

    /* Step 6: Canny边缘检测 */
    printf("[Step 6] Canny边缘检测 (thr_low=50, thr_high=120) ...\n");
    Image *edges=NULL;
    assert(ip_edge_canny(morphed, 50, 120, &edges)==IP_OK);
    assert(edges->fmt==IP_FMT_GRAY8);
    {
        Image *gr; ip_gray_to_rgb(edges,&gr);
        ip_bmp_save("output/pipe_canny.bmp", gr);
        ip_image_release(&gr);
    }
    /* 统计边缘像素占比（一般5%~40%之间为正常） */
    int edge_count=0;
    for (int i=0;i<320*240;i++) if (edges->data[i]==255) edge_count++;
    double ratio = 100.0*edge_count/(320*240);
    printf("  边缘像素占比=%.2f%% -> output/pipe_canny.bmp\n", ratio);
    assert(ratio > 1.0 && ratio < 60.0);

    ip_image_release(&morphed);
    ip_image_release(&edges);

    printf("\n========== test_pipeline: 全部通过 ==========\n");
    printf("  输出文件在 output/pipe_*.bmp，可直接打开查看完整流水线效果\n\n");
    return 0;
}
