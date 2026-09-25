/**
 * test_io.c —— IO与基础功能单元测试
 *
 * 验证点：
 *   - 图像创建/释放/克隆内存一致性
 *   - BMP保存→加载 round-trip 像素完全一致（MSE=0, PSNR=∞）
 *   - PPM(P6/P3) 保存→加载 round-trip
 *   - PGM(P5/P2) 保存→加载 round-trip
 *   - BMP 4字节行对齐处理正确性（宽度非4倍数）
 *   - 灰度↔RGB转换数据范围正确
 */
#include "imageproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>

static int test_create_release(void) {
    printf("[1] 测试创建/释放/克隆 ... ");
    Image *a=NULL, *b=NULL;
    assert(ip_image_create(10, 20, IP_FMT_GRAY8, &a) == IP_OK);
    assert(a && a->width==10 && a->height==20 && a->channels==1 && a->data_len==200);
    assert(ip_image_clone(a, &b) == IP_OK);
    /* 写入后clone应一致 */
    a->data[0] = 123; a->data[199] = 45;
    Image *c=NULL; assert(ip_image_clone(a,&c)==IP_OK);
    assert(c->data[0]==123 && c->data[199]==45);
    assert(c->data != a->data); /* 独立内存 */
    ip_image_release(&a); ip_image_release(&b); ip_image_release(&c);
    assert(a==NULL && b==NULL && c==NULL);
    printf("PASS\n"); return 1;
}

static int test_bmp_roundtrip(void) {
    printf("[2] 测试BMP save→load round-trip ... ");
    /* 测试多种宽度（覆盖4字节对齐的所有余数情况） */
    int widths[] = {1, 2, 3, 4, 5, 7, 8, 13, 100, 200, 233};
    for (int i=0; i<(int)(sizeof(widths)/sizeof(widths[0])); i++) {
        int w = widths[i];
        Image *src=NULL;
        assert(ip_create_test_pattern(w, 80, &src)==IP_OK);
        assert(ip_bmp_save("output/_test_rt.bmp", src)==IP_OK);
        Image *ld=NULL;
        assert(ip_bmp_load("output/_test_rt.bmp", &ld)==IP_OK);
        assert(ld->width==src->width && ld->height==src->height);
        double mse, psnr;
        ip_mse_psnr(src, ld, &mse, &psnr);
        /* BMP是无损的，必须MSE=0 */
        assert(mse == 0.0);
        assert(isinf(psnr));
        ip_image_release(&src); ip_image_release(&ld);
    }
    printf("PASS (11种宽度, 对齐正确)\n"); return 1;
}

static int test_ppm_roundtrip(void) {
    printf("[3] 测试PPM P6/P3 save→load round-trip ... ");
    Image *src=NULL; ip_create_test_pattern(100, 80, &src);
    assert(ip_ppm_save("output/_test_p6.ppm", src, 1)==IP_OK);
    Image *ld=NULL; assert(ip_ppm_load("output/_test_p6.ppm", &ld)==IP_OK);
    double mse,psnr; ip_mse_psnr(src,ld,&mse,&psnr); assert(mse==0.0);
    ip_image_release(&ld);
    assert(ip_ppm_save("output/_test_p3.ppm", src, 0)==IP_OK);
    assert(ip_ppm_load("output/_test_p3.ppm", &ld)==IP_OK);
    ip_mse_psnr(src,ld,&mse,&psnr); assert(mse==0.0);
    ip_image_release(&src); ip_image_release(&ld);
    printf("PASS\n"); return 1;
}

static int test_pgm_roundtrip(void) {
    printf("[4] 测试PGM P5/P2 save→load round-trip ... ");
    Image *src=NULL; ip_image_create(100,80,IP_FMT_GRAY8,&src);
    for (int i=0;i<100*80;i++) src->data[i] = (uint8_t)(i%256);
    assert(ip_pgm_save("output/_test_p5.pgm", src, 1)==IP_OK);
    Image *ld=NULL; assert(ip_pgm_load("output/_test_p5.pgm", &ld)==IP_OK);
    double mse; ip_mse_psnr(src,ld,&mse,NULL); assert(mse==0.0);
    ip_image_release(&ld);
    assert(ip_pgm_save("output/_test_p2.pgm", src, 0)==IP_OK);
    assert(ip_pgm_load("output/_test_p2.pgm", &ld)==IP_OK);
    ip_mse_psnr(src,ld,&mse,NULL); assert(mse==0.0);
    ip_image_release(&src); ip_image_release(&ld);
    printf("PASS\n"); return 1;
}

static int test_gray_rgb_conv(void) {
    printf("[5] 测试RGB↔Gray转换数据范围 ... ");
    Image *src=NULL; ip_image_create(10,10,IP_FMT_RGB24,&src);
    for (int i=0;i<100;i++){
        src->data[i*3+0]=200; src->data[i*3+1]=150; src->data[i*3+2]=100;
    }
    Image *g=NULL; ip_rgb_to_gray(src,&g);
    assert(g->fmt==IP_FMT_GRAY8);
    for (int i=0;i<100;i++){
        uint8_t v = g->data[i];
        /* NTSC: 0.299*200 + 0.587*150 + 0.114*100 = 59.8+88.05+11.4 ≈ 159 */
        assert(v >= 155 && v <= 165);
    }
    Image *rgb=NULL; ip_gray_to_rgb(g,&rgb);
    assert(rgb->channels==3);
    for (int i=0;i<100;i++){
        assert(IP_R(rgb,i%10,i/10)==g->data[i]);
        assert(IP_G(rgb,i%10,i/10)==g->data[i]);
        assert(IP_B(rgb,i%10,i/10)==g->data[i]);
    }
    ip_image_release(&src); ip_image_release(&g); ip_image_release(&rgb);
    printf("PASS\n"); return 1;
}

static int test_error_handling(void) {
    printf("[6] 测试错误参数处理（不崩溃） ... ");
    assert(ip_bmp_load("nonexistent_file.bmp", NULL) == IP_ERR_NULL_PTR);
    Image *x=NULL;
    assert(ip_bmp_load("nonexistent_file.bmp", &x) == IP_ERR_FILE_OPEN);
    assert(ip_bmp_save("/dev/null/", NULL) == IP_ERR_NULL_PTR);
    Image *bad;
    assert(ip_image_create(-1, 10, IP_FMT_GRAY8, &bad) == IP_ERR_NULL_PTR);
    printf("PASS\n"); return 1;
}

int main(void) {
    mkdir("output", 0755);
    int passed = 0;
    passed += test_create_release();
    passed += test_bmp_roundtrip();
    passed += test_ppm_roundtrip();
    passed += test_pgm_roundtrip();
    passed += test_gray_rgb_conv();
    passed += test_error_handling();
    printf("\n========== test_io: %d/6 全部通过 ==========\n\n", passed);
    return 0;
}
