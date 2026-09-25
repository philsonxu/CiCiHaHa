/**
 * filter.c —— 卷积核、方框/高斯/中值/拉普拉斯/Sobel/Canny边缘
 */
#include "imageproc.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

static inline uint8_t clamp255_d(double v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return (uint8_t)(v + 0.5);
}

void ip_kernel_gaussian(double *k, int ksize, double sigma) {
    if (!k) return;
    if (sigma <= 0) sigma = 0.3*((ksize-1)*0.5 - 1) + 0.8;
    int half = ksize / 2;
    double sum = 0;
    for (int y = -half; y <= half; y++) {
        for (int x = -half; x <= half; x++) {
            double v = exp(-(x*x+y*y)/(2*sigma*sigma)) / (2*M_PI*sigma*sigma);
            k[(y+half)*ksize + (x+half)] = v;
            sum += v;
        }
    }
    for (int i = 0; i < ksize*ksize; i++) k[i] /= sum;
}

/* ========== 3x3通用卷积（单通道/多通道均支持，边界复制填充） ========== */
IpStatus ip_conv3x3(const Image *src, Image **dst,
                    const double kernel[3][3], double divisor, double bias) {
    if (!src || !dst) return IP_ERR_NULL_PTR;
    int w = src->width, h = src->height, ch = src->channels;
    Image *d; IpStatus s = ip_image_create(w, h, src->fmt, &d);
    if (s != IP_OK) return s;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int c = 0; c < ch; c++) {
                double sum = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int yy = y+ky, xx = x+kx;
                        if (yy < 0) yy = 0;
                    if (yy >= h) yy = h-1;
                        if (xx < 0) xx = 0;
                    if (xx >= w) xx = w-1;
                        sum += src->data[((size_t)yy*w + xx)*ch + c] * kernel[ky+1][kx+1];
                    }
                }
                sum = sum/divisor + bias;
                d->data[((size_t)y*w + x)*ch + c] = clamp255_d(sum);
            }
        }
    }
    *dst = d; return IP_OK;
}

/* ========== 通用k×k方框滤波 ========== */
IpStatus ip_blur_box(const Image *src, int ksize, Image **dst) {
    if (!src || !dst || ksize<1) return IP_ERR_OUT_OF_RANGE;
    if (ksize%2==0) ksize++;
    int w = src->width, h = src->height, ch = src->channels;
    int half = ksize/2;
    Image *d; IpStatus s = ip_image_create(w, h, src->fmt, &d);
    if (s != IP_OK) return s;
    double div = (double)(ksize*ksize);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int c = 0; c < ch; c++) {
                double sum = 0;
                for (int ky=-half; ky<=half; ky++) {
                    for (int kx=-half; kx<=half; kx++) {
                        int yy=y+ky, xx=x+kx;
                        if(yy<0)yy=0;
                        if(yy>=h)yy=h-1;
                        if(xx<0)xx=0;
                        if(xx>=w)xx=w-1;
                        sum += src->data[((size_t)yy*w+xx)*ch+c];
                    }
                }
                d->data[((size_t)y*w+x)*ch+c] = clamp255_d(sum/div);
            }
        }
    }
    *dst = d; return IP_OK;
}

/* ========== k×k高斯滤波 ========== */
IpStatus ip_blur_gaussian(const Image *src, int ksize, double sigma, Image **dst) {
    if (!src || !dst || ksize<1) return IP_ERR_OUT_OF_RANGE;
    if (ksize%2==0) ksize++;
    int w = src->width, h = src->height, ch = src->channels;
    int half = ksize/2;
    double *k = (double*)malloc(sizeof(double)*ksize*ksize);
    if (!k) return IP_ERR_ALLOC;
    ip_kernel_gaussian(k, ksize, sigma);
    Image *d; IpStatus s = ip_image_create(w, h, src->fmt, &d);
    if (s != IP_OK) { free(k); return s; }
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            for (int c=0; c<ch; c++) {
                double sum = 0;
                for (int ky=-half; ky<=half; ky++) {
                    for (int kx=-half; kx<=half; kx++) {
                        int yy=y+ky, xx=x+kx;
                        if(yy<0)yy=0;
                        if(yy>=h)yy=h-1;
                        if(xx<0)xx=0;
                        if(xx>=w)xx=w-1;
                        sum += src->data[((size_t)yy*w+xx)*ch+c] * k[(ky+half)*ksize+(kx+half)];
                    }
                }
                d->data[((size_t)y*w+x)*ch+c] = clamp255_d(sum);
            }
        }
    }
    free(k);
    *dst = d; return IP_OK;
}

/* ========== 中值滤波（排序取中，对椒盐噪声极有效） ========== */
static int cmp_u8(const void *a, const void *b) {
    return *(const uint8_t*)a - *(const uint8_t*)b;
}
IpStatus ip_filter_median(const Image *src, int ksize, Image **dst) {
    if (!src || !dst || ksize<1) return IP_ERR_OUT_OF_RANGE;
    if (ksize%2==0) ksize++;
    int w = src->width, h = src->height, ch = src->channels;
    int half = ksize/2;
    int kn = ksize*ksize;
    uint8_t *win = (uint8_t*)malloc(kn);
    if (!win) return IP_ERR_ALLOC;
    Image *d; IpStatus s = ip_image_create(w, h, src->fmt, &d);
    if (s != IP_OK) { free(win); return s; }
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            for (int c=0; c<ch; c++) {
                int idx = 0;
                for (int ky=-half; ky<=half; ky++) {
                    for (int kx=-half; kx<=half; kx++) {
                        int yy=y+ky, xx=x+kx;
                        if(yy<0)yy=0;
                        if(yy>=h)yy=h-1;
                        if(xx<0)xx=0;
                        if(xx>=w)xx=w-1;
                        win[idx++] = src->data[((size_t)yy*w+xx)*ch+c];
                    }
                }
                qsort(win, (size_t)kn, 1, cmp_u8);
                d->data[((size_t)y*w+x)*ch+c] = win[kn/2];
            }
        }
    }
    free(win);
    *dst = d; return IP_OK;
}

/* ========== 拉普拉斯锐化 ========== */
IpStatus ip_sharpen_laplacian(const Image *src, Image **dst) {
    /* 3×3锐化核：中心5，四邻-1（图像减去拉普拉斯分量） */
    double k[3][3] = {{0,-1,0},{-1,5,-1},{0,-1,0}};
    return ip_conv3x3(src, dst, k, 1.0, 0);
}

/* ========== Sobel边缘检测（输出幅值与方向） ========== */
IpStatus ip_edge_sobel(const Image *gray, Image **mag_out, Image **dir_out) {
    if (!gray || !mag_out) return IP_ERR_NULL_PTR;
    if (gray->fmt != IP_FMT_GRAY8) return IP_ERR_INVALID_FMT;
    int w = gray->width, h = gray->height;
    Image *mag; IpStatus s = ip_image_create(w, h, IP_FMT_GRAY8, &mag);
    if (s != IP_OK) return s;
    Image *dir_img = NULL;
    if (dir_out) {
        s = ip_image_create(w, h, IP_FMT_GRAY8, &dir_img);
        if (s != IP_OK) { ip_image_release(&mag); return s; }
    }
    double gx_k[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    double gy_k[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            double gx=0, gy=0;
            for (int ky=-1; ky<=1; ky++) {
                for (int kx=-1; kx<=1; kx++) {
                    int yy=y+ky, xx=x+kx;
                    if(yy<0)yy=0;
                        if(yy>=h)yy=h-1;
                    if(xx<0)xx=0;
                        if(xx>=w)xx=w-1;
                    uint8_t v = gray->data[(size_t)yy*w+xx];
                    gx += v * gx_k[ky+1][kx+1];
                    gy += v * gy_k[ky+1][kx+1];
                }
            }
            double m = sqrt(gx*gx + gy*gy);
            mag->data[(size_t)y*w+x] = clamp255_d(m);
            if (dir_img) {
                double theta = atan2(gy, gx) * 180.0/M_PI; /* -180~180 */
                if (theta < 0) theta += 180;               /* 0~180 */
                dir_img->data[(size_t)y*w+x] = (uint8_t)(theta/180*255);
            }
        }
    }
    *mag_out = mag;
    if (dir_out) *dir_out = dir_img;
    return IP_OK;
}

/* ========== Canny边缘检测 ========== */
/* 步骤：高斯平滑 → Sobel梯度 → 非极大值抑制 → 双阈值滞后连接 */
IpStatus ip_edge_canny(const Image *gray, int thr_low, int thr_high, Image **edges) {
    if (!gray || !edges || gray->fmt != IP_FMT_GRAY8) return IP_ERR_INVALID_FMT;
    int w = gray->width, h = gray->height;
    /* 1) 先高斯平滑 */
    Image *blur = NULL;
    IpStatus s = ip_blur_gaussian(gray, 5, 1.4, &blur);
    if (s != IP_OK) return s;
    /* 2) 计算梯度和方向 */
    double *Gx = (double*)calloc((size_t)w*h, sizeof(double));
    double *Gy = (double*)calloc((size_t)w*h, sizeof(double));
    double *Gmag = (double*)calloc((size_t)w*h, sizeof(double));
    uint8_t *Gdir = (uint8_t*)calloc((size_t)w*h, 1); /* 0=0°,1=45°,2=90°,3=135° */
    if (!Gx||!Gy||!Gmag||!Gdir) { free(Gx);free(Gy);free(Gmag);free(Gdir); ip_image_release(&blur); return IP_ERR_ALLOC; }
    for (int y=1; y<h-1; y++) {
        for (int x=1; x<w-1; x++) {
            double gx = -IP_GRAY(blur,x-1,y-1) + IP_GRAY(blur,x+1,y-1)
                       -2*IP_GRAY(blur,x-1,y) + 2*IP_GRAY(blur,x+1,y)
                       -IP_GRAY(blur,x-1,y+1) + IP_GRAY(blur,x+1,y+1);
            double gy = -IP_GRAY(blur,x-1,y-1) -2*IP_GRAY(blur,x,y-1) -IP_GRAY(blur,x+1,y-1)
                        +IP_GRAY(blur,x-1,y+1) +2*IP_GRAY(blur,x,y+1) +IP_GRAY(blur,x+1,y+1);
            Gx[(size_t)y*w+x]=gx; Gy[(size_t)y*w+x]=gy;
            double m = sqrt(gx*gx+gy*gy);
            Gmag[(size_t)y*w+x]=m;
            double ang = atan2(gy,gx)*180/M_PI;
            if (ang < 0) ang += 180;
            /* 量化到4个方向 */
            if ((ang>=0 && ang<22.5) || (ang>=157.5 && ang<=180)) Gdir[(size_t)y*w+x]=0;
            else if (ang>=22.5 && ang<67.5)  Gdir[(size_t)y*w+x]=1;
            else if (ang>=67.5 && ang<112.5) Gdir[(size_t)y*w+x]=2;
            else                             Gdir[(size_t)y*w+x]=3;
        }
    }
    ip_image_release(&blur);
    /* 3) 非极大值抑制 */
    uint8_t *nms = (uint8_t*)calloc((size_t)w*h, 1);
    if (!nms) { free(Gx);free(Gy);free(Gmag);free(Gdir); return IP_ERR_ALLOC; }
    for (int y=1; y<h-1; y++) {
        for (int x=1; x<w-1; x++) {
            double m = Gmag[(size_t)y*w+x];
            int d = Gdir[(size_t)y*w+x];
            double n1=0, n2=0;
            if (d==0) { n1=Gmag[(size_t)y*w+(x-1)]; n2=Gmag[(size_t)y*w+(x+1)]; }
            else if (d==1) { n1=Gmag[(size_t)(y-1)*w+(x+1)]; n2=Gmag[(size_t)(y+1)*w+(x-1)]; }
            else if (d==2) { n1=Gmag[(size_t)(y-1)*w+x]; n2=Gmag[(size_t)(y+1)*w+x]; }
            else           { n1=Gmag[(size_t)(y-1)*w+(x-1)]; n2=Gmag[(size_t)(y+1)*w+(x+1)]; }
            if (m >= n1 && m >= n2) nms[(size_t)y*w+x] = 1;
        }
    }
    /* 4) 双阈值滞后连接：255=强边,128=弱边,0=非边 */
    uint8_t *edge = (uint8_t*)calloc((size_t)w*h, 1);
    if (!edge) { free(nms);free(Gx);free(Gy);free(Gmag);free(Gdir); return IP_ERR_ALLOC; }
    for (int i=0; i<w*h; i++) {
        double m = Gmag[i];
        if (nms[i] && m >= thr_high) edge[i] = 255;
        else if (nms[i] && m >= thr_low) edge[i] = 128;
    }
    /* 弱边若邻接强边则升级为强边（8连通，简单迭代一遍） */
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int y=1; y<h-1; y++) {
            for (int x=1; x<w-1; x++) {
                if (edge[(size_t)y*w+x] == 128) {
                    int strong = 0;
                    for (int dy=-1; dy<=1; dy++)
                        for (int dx=-1; dx<=1; dx++)
                            if (edge[(size_t)(y+dy)*w+(x+dx)] == 255) strong=1;
                    if (strong) { edge[(size_t)y*w+x] = 255; changed = 1; }
                    else { edge[(size_t)y*w+x] = 0; }
                }
            }
        }
    }
    Image *out; s = ip_image_create(w, h, IP_FMT_GRAY8, &out);
    if (s == IP_OK) memcpy(out->data, edge, (size_t)w*h);
    free(nms); free(Gx); free(Gy); free(Gmag); free(Gdir); free(edge);
    if (s != IP_OK) return s;
    *edges = out;
    return IP_OK;
}
