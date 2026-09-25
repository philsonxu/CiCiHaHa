/**
 * demo_morphology.c —— 示例4：二值化与形态学操作
 *
 * 流程：
 *   1. 灰度图 → Otsu自动二值化
 *   2. 加椒盐噪声 → 腐蚀/膨胀/开/闭运算对比
 *   3. 形态学组合去噪
 */
#include "imageproc.h"
#include <stdio.h>

int main(void) {
    printf("===== demo_morphology: 二值化与形态学 =====\n");
    Image *rgb = NULL; ip_create_test_pattern(400, 300, &rgb);

    /* 转灰度 */
    Image *gray = NULL; ip_rgb_to_gray(rgb, &gray);
    ip_image_release(&rgb);

    /* Otsu自动阈值 */
    uint8_t t = 0;
    ip_threshold_otsu(gray, &t);
    printf("Otsu自动阈值: t=%d\n", t);
    Image *bin = NULL; ip_image_clone(gray, &bin);
    ip_threshold(bin, t, 255);
    Image *bin_rgb=NULL; ip_gray_to_rgb(bin, &bin_rgb);
    ip_bmp_save("output/morph_otsu_binary.bmp", bin_rgb);
    printf("Otsu二值化 -> output/morph_otsu_binary.bmp\n");
    ip_image_release(&bin_rgb);

    /* 加椒盐噪声 */
    Image *noisy = NULL; ip_image_clone(bin, &noisy);
    ip_noise_salt_pepper(noisy, 0.05); /* 5%噪声 */
    Image *noisy_rgb=NULL; ip_gray_to_rgb(noisy, &noisy_rgb);
    ip_bmp_save("output/morph_noisy.bmp", noisy_rgb);
    printf("+5%%椒盐噪声 -> output/morph_noisy.bmp\n");
    ip_image_release(&noisy_rgb);

    /* 腐蚀 */
    Image *er = NULL; ip_morph_erode(noisy, 3, &er);
    Image *er_rgb=NULL; ip_gray_to_rgb(er, &er_rgb);
    ip_bmp_save("output/morph_erode3.bmp", er_rgb);
    printf("腐蚀3x3 -> output/morph_erode3.bmp\n");
    ip_image_release(&er); ip_image_release(&er_rgb);

    /* 膨胀 */
    Image *di = NULL; ip_morph_dilate(noisy, 3, &di);
    Image *di_rgb=NULL; ip_gray_to_rgb(di, &di_rgb);
    ip_bmp_save("output/morph_dilate3.bmp", di_rgb);
    printf("膨胀3x3 -> output/morph_dilate3.bmp\n");
    ip_image_release(&di); ip_image_release(&di_rgb);

    /* 开运算：去小噪点 */
    Image *op = NULL; ip_morph_open(noisy, 3, &op);
    Image *op_rgb=NULL; ip_gray_to_rgb(op, &op_rgb);
    ip_bmp_save("output/morph_open3.bmp", op_rgb);
    printf("开运算(去噪) -> output/morph_open3.bmp\n");
    ip_image_release(&op); ip_image_release(&op_rgb);

    /* 闭运算：填小黑洞 */
    Image *cl = NULL; ip_morph_close(noisy, 3, &cl);
    Image *cl_rgb=NULL; ip_gray_to_rgb(cl, &cl_rgb);
    ip_bmp_save("output/morph_close3.bmp", cl_rgb);
    printf("闭运算(填洞) -> output/morph_close3.bmp\n");
    ip_image_release(&cl); ip_image_release(&cl_rgb);

    ip_image_release(&gray); ip_image_release(&bin); ip_image_release(&noisy);
    printf("===== demo_morphology 完成 =====\n\n");
    return 0;
}
