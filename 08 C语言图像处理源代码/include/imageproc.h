/**
 * imageproc.h —— C语言图像处理库核心头文件
 *
 * 支持格式：
 *   - BMP 24位真彩色（Windows BITMAPINFOHEADER）
 *   - PPM P6/P3（二进制/文本）
 *   - PGM P5/P2（灰度图）
 *
 * 像素格式：
 *   - IP_FMT_GRAY8  : 8位灰度图，每像素1字节
 *   - IP_FMT_RGB24  : 24位RGB，每像素3字节 (R,G,B)
 */

#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========== 状态码 ========== */
typedef enum {
    IP_OK = 0,
    IP_ERR_NULL_PTR      = -1,  /* 空指针 */
    IP_ERR_FILE_OPEN     = -2,  /* 文件打开失败 */
    IP_ERR_INVALID_FMT   = -3,  /* 不支持的格式 */
    IP_ERR_CORRUPTED     = -4,  /* 文件损坏 */
    IP_ERR_ALLOC         = -5,  /* 内存分配失败 */
    IP_ERR_OUT_OF_RANGE  = -6,  /* 参数越界 */
    IP_ERR_DIM_MISMATCH  = -7   /* 尺寸/通道不匹配 */
} IpStatus;

/* ========== 像素格式 ========== */
typedef enum {
    IP_FMT_GRAY8 = 1,   /* 8位灰度 */
    IP_FMT_RGB24 = 3    /* 24位RGB */
} IpFormat;

/* ========== 图像结构体 ========== */
typedef struct {
    int width;          /* 宽度（像素） */
    int height;         /* 高度（像素） */
    int channels;       /* 通道数：1=Gray, 3=RGB */
    IpFormat fmt;       /* 像素格式 */
    uint8_t *data;      /* 像素数据，行序从上到下，列序从左到右，RGB按BGR存储兼容BMP */
    size_t   data_len;  /* 像素数据字节数 */
} Image;

/* ========== 图像创建与销毁 ========== */
IpStatus ip_image_create(int w, int h, IpFormat fmt, Image **out);
void     ip_image_release(Image **img);
IpStatus ip_image_clone(const Image *src, Image **dst);

/* ========== 文件IO（BMP/PPM/PGM） ========== */
IpStatus ip_bmp_load(const char *path, Image **out);
IpStatus ip_bmp_save(const char *path, const Image *img);
IpStatus ip_ppm_load(const char *path, Image **out);  /* P3/P6自动识别 */
IpStatus ip_ppm_save(const char *path, const Image *img, int binary); /* binary=1写P6 */
IpStatus ip_pgm_load(const char *path, Image **out);  /* P2/P5灰度 */
IpStatus ip_pgm_save(const char *path, const Image *img, int binary);

/* ========== 像素访问宏（内部统一RGB顺序） ========== */
#define IP_R(img, x, y)     ((img)->data[((y)*(img)->width + (x))*3 + 0])   /* R */
#define IP_G(img, x, y)     ((img)->data[((y)*(img)->width + (x))*3 + 1])   /* G */
#define IP_B(img, x, y)     ((img)->data[((y)*(img)->width + (x))*3 + 2])   /* B */
#define IP_GRAY(img, x, y)  ((img)->data[(y)*(img)->width + (x)])
#define IP_PIXEL(img, x, y) ((img)->data + ((y)*(img)->width + (x))*(img)->channels)

/* ========== 格式转换 ========== */
IpStatus ip_rgb_to_gray(const Image *src, Image **dst);
IpStatus ip_gray_to_rgb(const Image *src, Image **dst);

/* ========== 几何变换 ========== */
IpStatus ip_flip_horizontal(const Image *src, Image **dst);  /* 水平翻转 */
IpStatus ip_flip_vertical  (const Image *src, Image **dst);  /* 垂直翻转 */
IpStatus ip_rotate_90_cw   (const Image *src, Image **dst);  /* 顺时针90度 */
IpStatus ip_rotate_180     (const Image *src, Image **dst);  /* 旋转180度 */
IpStatus ip_resize_nearest (const Image *src, int nw, int nh, Image **dst); /* 最近邻缩放 */
IpStatus ip_resize_bilinear(const Image *src, int nw, int nh, Image **dst); /* 双线性插值缩放 */
IpStatus ip_crop           (const Image *src, int x0, int y0, int cw, int ch, Image **dst); /* 裁剪 */

/* ========== 亮度/对比度/色调调整 ========== */
IpStatus ip_brightness(Image *img, int delta);        /* 亮度调整 delta∈[-255,255] */
IpStatus ip_contrast  (Image *img, double factor);    /* 对比度 factor>0, 1.0为不变 */
IpStatus ip_invert    (Image *img);                   /* 反色 */
IpStatus ip_grayscale_avg(const Image *src, Image **dst);  /* 平均值法灰度 */
IpStatus ip_grayscale_ntsc(const Image *src, Image **dst); /* NTSC加权灰度 0.299R+0.587G+0.114B */

/* ========== 直方图 ========== */
IpStatus ip_histogram(const Image *gray, uint64_t hist[256]);
IpStatus ip_histogram_equalize(Image *gray);                     /* 直方图均衡化 */
IpStatus ip_histogram_save_ascii(const char *path, const uint64_t hist[256], int bar_h);

/* ========== 卷积与滤波 ========== */
/* 通用3×3卷积，kernel为3×3浮点数组 */
IpStatus ip_conv3x3(const Image *src, Image **dst,
                    const double kernel[3][3], double divisor, double bias);
IpStatus ip_blur_box(const Image *src, int ksize, Image **dst);     /* 均值/方框滤波 */
IpStatus ip_blur_gaussian(const Image *src, int ksize, double sigma, Image **dst); /* 高斯滤波 */
IpStatus ip_filter_median(const Image *src, int ksize, Image **dst);/* 中值滤波（去椒盐噪声） */
IpStatus ip_sharpen_laplacian(const Image *src, Image **dst);       /* 拉普拉斯锐化 */
IpStatus ip_edge_sobel(const Image *gray, Image **mag, Image **dir);/* Sobel边缘检测 */
IpStatus ip_edge_canny(const Image *gray, int thr_low, int thr_high, Image **edges); /* Canny边缘 */

/* ========== 二值化与形态学 ========== */
IpStatus ip_threshold(Image *gray, uint8_t t, uint8_t maxv);      /* 固定阈值二值化 */
IpStatus ip_threshold_otsu(const Image *gray, uint8_t *best_t);   /* 大津法自动阈值 */
IpStatus ip_morph_erode(const Image *bin, int ksize, Image **dst);   /* 腐蚀 */
IpStatus ip_morph_dilate(const Image *bin, int ksize, Image **dst);  /* 膨胀 */
IpStatus ip_morph_open(const Image *bin, int ksize, Image **dst);    /* 开运算（先腐后膨，去噪） */
IpStatus ip_morph_close(const Image *bin, int ksize, Image **dst);   /* 闭运算（先膨后腐，填洞） */

/* ========== 图像绘制（简单图形） ========== */
IpStatus ip_draw_pixel(Image *img, int x, int y, uint8_t r, uint8_t g, uint8_t b);
IpStatus ip_draw_line (Image *img, int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b); /* Bresenham */
IpStatus ip_draw_rect (Image *img, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, int filled);
IpStatus ip_draw_circle(Image *img, int cx, int cy, int rad, uint8_t r, uint8_t g, uint8_t b, int filled); /* 中点圆 */

/* ========== 噪声与统计 ========== */
IpStatus ip_noise_gaussian(Image *img, double sigma);       /* 加高斯噪声 */
IpStatus ip_noise_salt_pepper(Image *img, double prob);     /* 加椒盐噪声 */
IpStatus ip_mse_psnr(const Image *a, const Image *b, double *mse, double *psnr); /* 均方误差与峰值信噪比 */
IpStatus ip_mean_stddev(const Image *gray, double *mean, double *stddev);  /* 均值与标准差 */

/* ========== 卷积核生成 ========== */
void ip_kernel_gaussian(double *k, int ksize, double sigma);

/* ========== 工具函数 ========== */
const char* ip_status_str(IpStatus s);
void ip_image_info(const Image *img);
/* 生成一张纯测试图像：渐变色+几何图形，无需外部文件即可验证 */
IpStatus ip_create_test_pattern(int w, int h, Image **out);

#ifdef __cplusplus
}
#endif

#endif /* IMAGEPROC_H */
