/**
 * noise_stat.c —— 噪声生成（高斯/椒盐）、PSNR/MSE、均值/标准差
 */
#include "imageproc.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* Box-Muller变换生成标准正态随机数 */
static double randn() {
    static int have = 0;
    static double z;
    if (have) { have = 0; return z; }
    double u1, u2;
    do { u1 = (rand()+1.0)/(RAND_MAX+1.0); u2 = (rand()+1.0)/(RAND_MAX+1.0); } while (u1 <= 1e-10);
    double r = sqrt(-2*log(u1));
    z = r*sin(2*M_PI*u2);
    have = 1;
    return r*cos(2*M_PI*u2);
}

static inline uint8_t clamp8(double v){
    if (v<0) return 0;
    if (v>255) return 255;
    return (uint8_t)(v+0.5);
}

IpStatus ip_noise_gaussian(Image *img, double sigma) {
    if (!img) return IP_ERR_NULL_PTR;
    srand((unsigned)time(NULL));
    int n = (int)img->data_len;
    for (int i=0; i<n; i++) {
        double v = img->data[i] + sigma * randn();
        img->data[i] = clamp8(v);
    }
    return IP_OK;
}

IpStatus ip_noise_salt_pepper(Image *img, double prob) {
    if (!img) return IP_ERR_NULL_PTR;
    srand((unsigned)time(NULL)+1);
    int ch = img->channels;
    /* 按像素添加（不是按通道，保持RGB一致） */
    for (int i=0; i<img->width*img->height; i++) {
        double r = (double)rand()/RAND_MAX;
        if (r < prob) {
            for (int c=0; c<ch; c++) img->data[i*ch+c] = 0;   /* 椒 */
        } else if (r > 1-prob) {
            for (int c=0; c<ch; c++) img->data[i*ch+c] = 255; /* 盐 */
        }
    }
    return IP_OK;
}

IpStatus ip_mse_psnr(const Image *a, const Image *b, double *mse, double *psnr) {
    if (!a || !b) return IP_ERR_NULL_PTR;
    if (a->width!=b->width || a->height!=b->height || a->fmt!=b->fmt) return IP_ERR_DIM_MISMATCH;
    int n = a->width*a->height*a->channels;
    double sum=0;
    for (int i=0; i<n; i++) {
        double d = (double)a->data[i] - b->data[i];
        sum += d*d;
    }
    double m = sum/n;
    if (mse)  *mse = m;
    if (psnr) *psnr = (m == 0) ? INFINITY : 10*log10(255*255/m);
    return IP_OK;
}

IpStatus ip_mean_stddev(const Image *gray, double *mean, double *stddev) {
    if (!gray || gray->fmt!=IP_FMT_GRAY8) return IP_ERR_INVALID_FMT;
    int n = gray->width*gray->height;
    double s1=0, s2=0;
    for (int i=0; i<n; i++) { s1 += gray->data[i]; s2 += (double)gray->data[i]*gray->data[i]; }
    double m = s1/n;
    if (mean) *mean = m;
    if (stddev) *stddev = sqrt(s2/n - m*m);
    return IP_OK;
}
