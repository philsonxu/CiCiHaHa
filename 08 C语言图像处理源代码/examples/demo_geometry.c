/**
 * demo_geometry.c —— 示例6：几何变换（翻转/旋转/缩放/裁剪）
 */
#include "imageproc.h"
#include <stdio.h>

int main(void) {
    printf("===== demo_geometry: 几何变换 =====\n");
    Image *src; ip_create_test_pattern(400, 300, &src);
    ip_bmp_save("output/geo_original.bmp", src);
    printf("原图 400x300 -> output/geo_original.bmp\n");

    /* 水平翻转 */
    Image *fh; ip_flip_horizontal(src, &fh);
    ip_bmp_save("output/geo_flip_h.bmp", fh);
    printf("水平翻转 -> output/geo_flip_h.bmp\n"); ip_image_release(&fh);

    /* 垂直翻转 */
    Image *fv; ip_flip_vertical(src, &fv);
    ip_bmp_save("output/geo_flip_v.bmp", fv);
    printf("垂直翻转 -> output/geo_flip_v.bmp\n"); ip_image_release(&fv);

    /* 顺时针90度 */
    Image *r90; ip_rotate_90_cw(src, &r90);
    ip_bmp_save("output/geo_rotate90.bmp", r90);
    printf("顺时针90度 -> output/geo_rotate90.bmp (尺寸%d×%d)\n", r90->width, r90->height);
    ip_image_release(&r90);

    /* 180度 */
    Image *r180; ip_rotate_180(src, &r180);
    ip_bmp_save("output/geo_rotate180.bmp", r180);
    printf("旋转180度 -> output/geo_rotate180.bmp\n"); ip_image_release(&r180);

    /* 最近邻缩小 */
    Image *nn; ip_resize_nearest(src, 200, 150, &nn);
    ip_bmp_save("output/geo_resize_nn_200x150.bmp", nn);
    printf("最近邻缩放 200x150 -> output/geo_resize_nn_200x150.bmp\n"); ip_image_release(&nn);

    /* 双线性放大 */
    Image *bl; ip_resize_bilinear(src, 800, 600, &bl);
    ip_bmp_save("output/geo_resize_bilinear_800x600.bmp", bl);
    printf("双线性插值放大 800x600 -> output/geo_resize_bilinear_800x600.bmp\n"); ip_image_release(&bl);

    /* 最近邻放大（像素感） */
    Image *nn2; ip_resize_nearest(src, 800, 600, &nn2);
    ip_bmp_save("output/geo_resize_nn_800x600.bmp", nn2);
    printf("最近邻放大 800x600(像素风) -> output/geo_resize_nn_800x600.bmp\n"); ip_image_release(&nn2);

    /* 裁剪中心区域 */
    Image *cr; ip_crop(src, 100, 75, 200, 150, &cr);
    ip_bmp_save("output/geo_crop_center.bmp", cr);
    printf("裁剪中心200x150 -> output/geo_crop_center.bmp\n"); ip_image_release(&cr);

    ip_image_release(&src);
    printf("===== demo_geometry 完成 =====\n\n");
    return 0;
}
