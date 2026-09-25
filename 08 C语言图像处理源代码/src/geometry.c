/**
 * geometry.c —— 几何变换：翻转、旋转、缩放（最近邻/双线性）、裁剪
 */
#include "imageproc.h"
#include <stdlib.h>
#include <string.h>

IpStatus ip_flip_horizontal(const Image *src, Image **dst) {
    if (!src || !dst) return IP_ERR_NULL_PTR;
    Image *d; IpStatus s = ip_image_create(src->width, src->height, src->fmt, &d);
    if (s != IP_OK) return s;
    int w = src->width, h = src->height, ch = src->channels;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            const uint8_t *sp = src->data + ((size_t)y*w + (w-1-x))*ch;
            uint8_t *dp = d->data + ((size_t)y*w + x)*ch;
            for (int c = 0; c < ch; c++) dp[c] = sp[c];
        }
    }
    *dst = d; return IP_OK;
}

IpStatus ip_flip_vertical(const Image *src, Image **dst) {
    if (!src || !dst) return IP_ERR_NULL_PTR;
    Image *d; IpStatus s = ip_image_create(src->width, src->height, src->fmt, &d);
    if (s != IP_OK) return s;
    int w = src->width, h = src->height, ch = src->channels;
    for (int y = 0; y < h; y++) {
        memcpy(d->data + (size_t)y*w*ch,
               src->data + (size_t)(h-1-y)*w*ch,
               (size_t)w*ch);
    }
    *dst = d; return IP_OK;
}

IpStatus ip_rotate_90_cw(const Image *src, Image **dst) {
    if (!src || !dst) return IP_ERR_NULL_PTR;
    /* 旋转后尺寸交换 */
    int w = src->width, h = src->height, ch = src->channels;
    Image *d; IpStatus s = ip_image_create(h, w, src->fmt, &d);
    if (s != IP_OK) return s;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            /* 新位置 (y, w-1-x) ← 原位置 (x, y)  对于顺时针90：
               原(x,y) → 新(y, w-1-x) */
            const uint8_t *sp = src->data + ((size_t)y*w + x)*ch;
            uint8_t *dp = d->data + ((size_t)x*h + (h-1-y))*ch;
            for (int c = 0; c < ch; c++) dp[c] = sp[c];
        }
    }
    *dst = d; return IP_OK;
}

IpStatus ip_rotate_180(const Image *src, Image **dst) {
    if (!src || !dst) return IP_ERR_NULL_PTR;
    int w = src->width, h = src->height, ch = src->channels;
    Image *d; IpStatus s = ip_image_create(w, h, src->fmt, &d);
    if (s != IP_OK) return s;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            const uint8_t *sp = src->data + ((size_t)y*w + x)*ch;
            uint8_t *dp = d->data + ((size_t)(h-1-y)*w + (w-1-x))*ch;
            for (int c = 0; c < ch; c++) dp[c] = sp[c];
        }
    }
    *dst = d; return IP_OK;
}

IpStatus ip_resize_nearest(const Image *src, int nw, int nh, Image **dst) {
    if (!src || !dst || nw<=0 || nh<=0) return IP_ERR_OUT_OF_RANGE;
    int w = src->width, h = src->height, ch = src->channels;
    Image *d; IpStatus s = ip_image_create(nw, nh, src->fmt, &d);
    if (s != IP_OK) return s;
    for (int y = 0; y < nh; y++) {
        int sy = y * h / nh;
        if (sy >= h) sy = h-1;
        for (int x = 0; x < nw; x++) {
            int sx = x * w / nw;
            if (sx >= w) sx = w-1;
            const uint8_t *sp = src->data + ((size_t)sy*w + sx)*ch;
            uint8_t *dp = d->data + ((size_t)y*nw + x)*ch;
            for (int c = 0; c < ch; c++) dp[c] = sp[c];
        }
    }
    *dst = d; return IP_OK;
}

IpStatus ip_resize_bilinear(const Image *src, int nw, int nh, Image **dst) {
    if (!src || !dst || nw<=0 || nh<=0) return IP_ERR_OUT_OF_RANGE;
    int w = src->width, h = src->height, ch = src->channels;
    Image *d; IpStatus s = ip_image_create(nw, nh, src->fmt, &d);
    if (s != IP_OK) return s;
    double x_ratio = (w-1) / (double)nw;
    double y_ratio = (h-1) / (double)nh;
    for (int y = 0; y < nh; y++) {
        double fy = y * y_ratio;
        int y0 = (int)fy; if (y0 >= h-1) y0 = h-2;
        int y1 = y0 + 1;
        double wy = fy - y0;
        for (int x = 0; x < nw; x++) {
            double fx = x * x_ratio;
            int x0 = (int)fx; if (x0 >= w-1) x0 = w-2;
            int x1 = x0 + 1;
            double wx = fx - x0;
            uint8_t *dp = d->data + ((size_t)y*nw + x)*ch;
            for (int c = 0; c < ch; c++) {
                double v00 = src->data[((size_t)y0*w + x0)*ch + c];
                double v10 = src->data[((size_t)y0*w + x1)*ch + c];
                double v01 = src->data[((size_t)y1*w + x0)*ch + c];
                double v11 = src->data[((size_t)y1*w + x1)*ch + c];
                double v = v00*(1-wx)*(1-wy) + v10*wx*(1-wy) + v01*(1-wx)*wy + v11*wx*wy;
                dp[c] = (uint8_t)(v + 0.5);
            }
        }
    }
    *dst = d; return IP_OK;
}

IpStatus ip_crop(const Image *src, int x0, int y0, int cw, int ch, Image **dst) {
    if (!src || !dst) return IP_ERR_NULL_PTR;
    if (x0<0 || y0<0 || cw<=0 || ch<=0) return IP_ERR_OUT_OF_RANGE;
    if (x0+cw > src->width || y0+ch > src->height) return IP_ERR_OUT_OF_RANGE;
    Image *d; IpStatus s = ip_image_create(cw, ch, src->fmt, &d);
    if (s != IP_OK) return s;
    int sc = src->channels;
    for (int y = 0; y < ch; y++) {
        memcpy(d->data + (size_t)y*cw*sc,
               src->data + ((size_t)(y0+y)*src->width + x0)*sc,
               (size_t)cw*sc);
    }
    *dst = d; return IP_OK;
}
