/**
 * core.c —— 图像创建/释放/克隆、状态字符串、测试图案生成、图像信息打印
 */
#include "imageproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* ip_status_str(IpStatus s) {
    switch (s) {
        case IP_OK:              return "OK";
        case IP_ERR_NULL_PTR:    return "Null pointer";
        case IP_ERR_FILE_OPEN:   return "File open failed";
        case IP_ERR_INVALID_FMT: return "Unsupported format";
        case IP_ERR_CORRUPTED:   return "File corrupted";
        case IP_ERR_ALLOC:       return "Memory allocation failed";
        case IP_ERR_OUT_OF_RANGE:return "Parameter out of range";
        case IP_ERR_DIM_MISMATCH:return "Dimension/channel mismatch";
        default:                 return "Unknown error";
    }
}

IpStatus ip_image_create(int w, int h, IpFormat fmt, Image **out) {
    if (!out || w <= 0 || h <= 0) return IP_ERR_NULL_PTR;
    if (fmt != IP_FMT_GRAY8 && fmt != IP_FMT_RGB24) return IP_ERR_INVALID_FMT;

    Image *img = (Image*)calloc(1, sizeof(Image));
    if (!img) return IP_ERR_ALLOC;
    int ch = (int)fmt;
    size_t len = (size_t)w * h * ch;
    img->data = (uint8_t*)calloc(len, 1);
    if (!img->data) { free(img); return IP_ERR_ALLOC; }
    img->width = w; img->height = h;
    img->channels = ch; img->fmt = fmt;
    img->data_len = len;
    *out = img;
    return IP_OK;
}

void ip_image_release(Image **img) {
    if (!img || !*img) return;
    if ((*img)->data) free((*img)->data);
    free(*img);
    *img = NULL;
}

IpStatus ip_image_clone(const Image *src, Image **dst) {
    if (!src || !dst) return IP_ERR_NULL_PTR;
    IpStatus s = ip_image_create(src->width, src->height, src->fmt, dst);
    if (s != IP_OK) return s;
    memcpy((*dst)->data, src->data, src->data_len);
    return IP_OK;
}

void ip_image_info(const Image *img) {
    if (!img) { printf("[NULL image]\n"); return; }
    printf("Image: %dx%d, channels=%d, fmt=%s, data=%zu bytes\n",
           img->width, img->height, img->channels,
           img->fmt == IP_FMT_GRAY8 ? "GRAY8" : "RGB24",
           img->data_len);
}

IpStatus ip_create_test_pattern(int w, int h, Image **out) {
    IpStatus s = ip_image_create(w, h, IP_FMT_RGB24, out);
    if (s != IP_OK) return s;
    Image *img = *out;
    /* 背景：渐变 RGB（左上红到右下蓝） */
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            IP_R(img,x,y) = (uint8_t)(255.0 * x / (w-1));
            IP_G(img,x,y) = (uint8_t)(255.0 * y / (h-1));
            IP_B(img,x,y) = (uint8_t)(128);
        }
    }
    /* 中心白色圆 */
    int cx = w/2, cy = h/2, rad = (w<h?w:h)/5;
    ip_draw_circle(img, cx, cy, rad, 255, 255, 255, 1);
    /* 左上红色矩形 */
    ip_draw_rect(img, 20, 20, w/5, h/5, 255, 0, 0, 1);
    /* 右下蓝色矩形边框 */
    ip_draw_rect(img, w - w/5 - 20, h - h/5 - 20, w/5, h/5, 0, 0, 255, 0);
    /* 对角绿色直线 */
    ip_draw_line(img, 0, 0, w-1, h-1, 0, 255, 0);
    /* 另一条对角黄色 */
    ip_draw_line(img, 0, h-1, w-1, 0, 255, 255, 0);
    /* 中心黑点 */
    ip_draw_circle(img, cx, cy, 4, 0, 0, 0, 1);
    return IP_OK;
}
