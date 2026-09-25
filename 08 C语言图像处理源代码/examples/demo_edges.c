/**
 * demo_edges.c —— 示例3：边缘检测（Sobel + Canny）
 */
#include "imageproc.h"
#include <stdio.h>

int main(void) {
    printf("===== demo_edges: 边缘检测 =====\n");
    Image *rgb = NULL;
    ip_create_test_pattern(400, 300, &rgb);
    ip_bmp_save("output/edge_input.bmp", rgb);

    /* 转灰度 */
    Image *gray = NULL; ip_rgb_to_gray(rgb, &gray);
    Image *gray_rgb = NULL; ip_gray_to_rgb(gray, &gray_rgb);
    ip_bmp_save("output/edge_gray.bmp", gray_rgb);
    ip_image_release(&gray_rgb);

    /* Sobel */
    Image *sob = NULL, *dir=NULL;
    ip_edge_sobel(gray, &sob, &dir);
    Image *sob_rgb = NULL; ip_gray_to_rgb(sob, &sob_rgb);
    ip_bmp_save("output/edge_sobel.bmp", sob_rgb);
    printf("Sobel幅值 -> output/edge_sobel.bmp\n");
    if (dir) {
        Image *dir_rgb=NULL; ip_gray_to_rgb(dir, &dir_rgb);
        ip_bmp_save("output/edge_sobel_dir.bmp", dir_rgb);
        printf("Sobel方向 -> output/edge_sobel_dir.bmp\n");
        ip_image_release(&dir); ip_image_release(&dir_rgb);
    }
    ip_image_release(&sob); ip_image_release(&sob_rgb);

    /* Canny（低阈值50，高阈值100） */
    Image *canny = NULL;
    ip_edge_canny(gray, 50, 100, &canny);
    Image *canny_rgb=NULL; ip_gray_to_rgb(canny, &canny_rgb);
    ip_bmp_save("output/edge_canny.bmp", canny_rgb);
    printf("Canny(50,100) -> output/edge_canny.bmp\n");
    ip_image_release(&canny); ip_image_release(&canny_rgb);

    /* Canny（高低阈值对比） */
    Image *canny2 = NULL;
    ip_edge_canny(gray, 20, 60, &canny2);
    Image *canny2_rgb=NULL; ip_gray_to_rgb(canny2, &canny2_rgb);
    ip_bmp_save("output/edge_canny_loose.bmp", canny2_rgb);
    printf("Canny(20,60)更宽松阈值 -> output/edge_canny_loose.bmp\n");
    ip_image_release(&canny2); ip_image_release(&canny2_rgb);

    ip_image_release(&rgb); ip_image_release(&gray);
    printf("===== demo_edges 完成 =====\n\n");
    return 0;
}
