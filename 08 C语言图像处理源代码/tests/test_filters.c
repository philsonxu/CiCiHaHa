/**
 * test_filters.c —— 滤波与卷积数学正确性验证
 *
 * 验证点：
 *   - 恒等卷积核（中心1其余0）输出=输入
 *   - 均值滤波对均匀区域无影响
 *   - 高斯滤波能量守恒（核和=1）
 *   - 反色再反色还原
 *   - 亮度+delta再-delta还原
 *   - 水平+垂直翻转组合=180旋转
 *   - 放大再缩小后PSNR可接受
 *   - 中值滤波对椒盐噪声去除效果显著（PSNR大幅提升）
 */
#include "imageproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>

static int test_identity_kernel(void) {
    printf("[1] 恒等卷积核输出=输入 ... ");
    Image *src=NULL; ip_create_test_pattern(100,80,&src);
    double id[3][3]={{0,0,0},{0,1,0},{0,0,0}};
    Image *out=NULL; ip_conv3x3(src, &out, id, 1.0, 0);
    double mse; ip_mse_psnr(src,out,&mse,NULL);
    assert(mse == 0.0);
    ip_image_release(&src); ip_image_release(&out);
    printf("PASS\n"); return 1;
}

static int test_box_constant(void) {
    printf("[2] 均值滤波对纯色无影响 ... ");
    Image *src=NULL; ip_image_create(60,60,IP_FMT_RGB24,&src);
    for (int i=0;i<60*60*3;i++) src->data[i]=128;
    Image *out=NULL; ip_blur_box(src, 7, &out);
    double mse; ip_mse_psnr(src,out,&mse,NULL);
    assert(mse == 0.0);
    ip_image_release(&src); ip_image_release(&out);
    printf("PASS\n"); return 1;
}

static int test_gaussian_kernel_sum(void) {
    printf("[3] 高斯核和=1（能量守恒） ... ");
    double k[81];
    for (int ks=3; ks<=9; ks+=2) {
        ip_kernel_gaussian(k, ks, 1.0);
        double s=0;
        for (int i=0;i<ks*ks;i++) s+=k[i];
        assert(fabs(s-1.0) < 1e-9);
    }
    printf("PASS (ksize=3/5/7/9)\n"); return 1;
}

static int test_invert_twice(void) {
    printf("[4] 反色再反色还原 ... ");
    Image *src=NULL, *a=NULL;
    ip_create_test_pattern(80,60,&src);
    ip_image_clone(src,&a);
    ip_invert(a);
    /* 检查确实变了 */
    double mse; ip_mse_psnr(src,a,&mse,NULL); assert(mse > 1000);
    ip_invert(a);
    ip_mse_psnr(src,a,&mse,NULL); assert(mse == 0.0);
    ip_image_release(&src); ip_image_release(&a);
    printf("PASS\n"); return 1;
}

static int test_brightness_restore(void) {
    printf("[5] 亮度+delta再-delta还原 ... ");
    Image *src=NULL, *a=NULL;
    /* 用中间范围的灰度图避免clamp截断 */
    ip_image_create(80,60,IP_FMT_GRAY8,&src);
    for(int i=0;i<80*60;i++) src->data[i] = 100 + (i%56); /* 范围[100,155]，+/-40都不碰边界 */
    ip_image_clone(src,&a);
    ip_brightness(a, 40);
    ip_brightness(a,-40);
    double mse; ip_mse_psnr(src,a,&mse,NULL);
    assert(mse < 1.0);
    ip_image_release(&src); ip_image_release(&a);
    printf("PASS (MSE=%.3f)\n", mse); return 1;
}

static int test_flips_compose(void) {
    printf("[6] 水平+垂直翻转 == 180度旋转 ... ");
    Image *src=NULL; ip_create_test_pattern(80,60,&src);
    Image *fh=NULL, *fhv=NULL, *r180=NULL;
    ip_flip_horizontal(src, &fh);
    ip_flip_vertical(fh, &fhv);
    ip_rotate_180(src, &r180);
    double mse; ip_mse_psnr(fhv, r180, &mse, NULL);
    assert(mse == 0.0);
    ip_image_release(&src); ip_image_release(&fh); ip_image_release(&fhv); ip_image_release(&r180);
    printf("PASS\n"); return 1;
}

static int test_resize_cycle(void) {
    printf("[7] 放大再缩小保真度(双线性) ... ");
    Image *src=NULL; ip_create_test_pattern(100,80,&src);
    Image *big=NULL, *bk=NULL;
    /* 2倍放大再缩回原尺寸 */
    ip_resize_bilinear(src, 200, 160, &big);
    ip_resize_bilinear(big, 100, 80, &bk);
    double mse,psnr; ip_mse_psnr(src,bk,&mse,&psnr);
    assert(psnr > 15.0);
    ip_image_release(&src); ip_image_release(&big); ip_image_release(&bk);
    printf("PASS (PSNR=%.2f dB > 15dB)\n", psnr); return 1;
}

static int test_median_sp(void) {
    printf("[8] 中值滤波对椒盐噪声PSNR显著提升 ... ");
    Image *rgb=NULL; ip_create_test_pattern(200,150,&rgb);
    Image *gray=NULL; ip_rgb_to_gray(rgb,&gray);
    Image *noisy=NULL; ip_image_clone(gray,&noisy);
    ip_noise_salt_pepper(noisy, 0.1);
    double mse_before, psnr_before, mse_after, psnr_after;
    ip_mse_psnr(gray, noisy, &mse_before, &psnr_before);
    Image *den=NULL; ip_filter_median(noisy,3,&den);
    ip_mse_psnr(gray, den, &mse_after, &psnr_after);
    assert(psnr_after > psnr_before + 10);
    ip_image_release(&rgb); ip_image_release(&gray); ip_image_release(&noisy); ip_image_release(&den);
    printf("PASS (噪声:%.2f dB -> 中值:%.2f dB, 提升%.1f dB)\n",
           psnr_before, psnr_after, psnr_after-psnr_before); return 1;
}

static int test_crop_outofrange(void) {
    printf("[9] 越界裁剪参数正确报错 ... ");
    Image *src=NULL; ip_create_test_pattern(100,80,&src);
    Image *out=NULL;
    assert(ip_crop(src, 50, 50, 200, 200, &out) == IP_ERR_OUT_OF_RANGE);
    assert(ip_crop(src, -1, 0, 10, 10, &out) == IP_ERR_OUT_OF_RANGE);
    assert(ip_crop(src, 0, 0, 0, 10, &out) == IP_ERR_OUT_OF_RANGE);
    assert(ip_crop(src, 0, 0, 50, 50, &out) == IP_OK);
    assert(out->width==50 && out->height==50);
    ip_image_release(&src); ip_image_release(&out);
    printf("PASS\n"); return 1;
}

int main(void) {
    mkdir("output", 0755);
    int passed = 0;
    passed += test_identity_kernel();
    passed += test_box_constant();
    passed += test_gaussian_kernel_sum();
    passed += test_invert_twice();
    passed += test_brightness_restore();
    passed += test_flips_compose();
    passed += test_resize_cycle();
    passed += test_median_sp();
    passed += test_crop_outofrange();
    printf("\n========== test_filters: %d/9 全部通过 ==========\n\n", passed);
    return 0;
}
