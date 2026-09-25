/**
 * adjust.c —— 格式转换、亮度/对比度/反色、灰度转换
 */
#include "imageproc.h"
#include <stdlib.h>
#include <string.h>

static inline uint8_t clamp255(int v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return (uint8_t)v;
}

IpStatus ip_rgb_to_gray(const Image *src, Image **dst) {
    if (!src || !dst || src->fmt != IP_FMT_RGB24) return IP_ERR_INVALID_FMT;
    int w = src->width, h = src->height;
    Image *g = NULL;
    IpStatus s = ip_image_create(w, h, IP_FMT_GRAY8, &g);
    if (s != IP_OK) return s;
    /* NTSC加权：Y = 0.299R + 0.587G + 0.114B，用整数近似避免浮点 */
    for (int i = 0; i < w*h; i++) {
        int R = src->data[i*3+0];
        int G = src->data[i*3+1];
        int B = src->data[i*3+2];
        g->data[i] = (uint8_t)((R*30 + G*59 + B*11 + 50) / 100);
    }
    *dst = g;
    return IP_OK;
}

IpStatus ip_gray_to_rgb(const Image *src, Image **dst) {
    if (!src || !dst || src->fmt != IP_FMT_GRAY8) return IP_ERR_INVALID_FMT;
    int w = src->width, h = src->height;
    Image *rgb = NULL;
    IpStatus s = ip_image_create(w, h, IP_FMT_RGB24, &rgb);
    if (s != IP_OK) return s;
    for (int i = 0; i < w*h; i++) {
        uint8_t v = src->data[i];
        rgb->data[i*3+0] = v;
        rgb->data[i*3+1] = v;
        rgb->data[i*3+2] = v;
    }
    *dst = rgb;
    return IP_OK;
}

IpStatus ip_grayscale_avg(const Image *src, Image **dst) {
    if (!src || !dst || src->fmt != IP_FMT_RGB24) return IP_ERR_INVALID_FMT;
    int w = src->width, h = src->height;
    Image *g; IpStatus s = ip_image_create(w, h, IP_FMT_GRAY8, &g);
    if (s != IP_OK) return s;
    for (int i = 0; i < w*h; i++) {
        int R = src->data[i*3+0], G = src->data[i*3+1], B = src->data[i*3+2];
        g->data[i] = (uint8_t)((R+G+B)/3);
    }
    *dst = g; return IP_OK;
}

IpStatus ip_grayscale_ntsc(const Image *src, Image **dst) {
    return ip_rgb_to_gray(src, dst);
}

IpStatus ip_brightness(Image *img, int delta) {
    if (!img) return IP_ERR_NULL_PTR;
    int n = (int)img->data_len;
    for (int i = 0; i < n; i++) {
        img->data[i] = clamp255(img->data[i] + delta);
    }
    return IP_OK;
}

IpStatus ip_contrast(Image *img, double factor) {
    if (!img) return IP_ERR_NULL_PTR;
    if (factor <= 0) return IP_ERR_OUT_OF_RANGE;
    int n = (int)img->data_len;
    for (int i = 0; i < n; i++) {
        /* 以128为中心做缩放 */
        int v = (int)(factor * (img->data[i] - 128) + 128);
        img->data[i] = clamp255(v);
    }
    return IP_OK;
}

IpStatus ip_invert(Image *img) {
    if (!img) return IP_ERR_NULL_PTR;
    int n = (int)img->data_len;
    for (int i = 0; i < n; i++) img->data[i] = (uint8_t)(255 - img->data[i]);
    return IP_OK;
}
