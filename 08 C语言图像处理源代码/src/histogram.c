/**
 * histogram.c —— 直方图统计、均衡化、ASCII可视化
 */
#include "imageproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IpStatus ip_histogram(const Image *gray, uint64_t hist[256]) {
    if (!gray || !hist) return IP_ERR_NULL_PTR;
    if (gray->fmt != IP_FMT_GRAY8) return IP_ERR_INVALID_FMT;
    memset(hist, 0, sizeof(uint64_t)*256);
    int n = gray->width * gray->height;
    for (int i = 0; i < n; i++) hist[gray->data[i]]++;
    return IP_OK;
}

IpStatus ip_histogram_equalize(Image *gray) {
    if (!gray) return IP_ERR_NULL_PTR;
    if (gray->fmt != IP_FMT_GRAY8) return IP_ERR_INVALID_FMT;
    uint64_t hist[256] = {0};
    int n = gray->width * gray->height;
    for (int i = 0; i < n; i++) hist[gray->data[i]]++;

    /* 累计分布函数CDF */
    uint8_t lut[256];
    uint64_t cdf = 0;
    int mincdf = 0; /* 跳过频数为0的灰度级找到最小cdf */
    for (int i = 0; i < 256; i++) {
        if (hist[i] > 0) { mincdf = (int)hist[i]; break; }
    }
    int denom = n - mincdf;
    if (denom <= 0) return IP_OK; /* 全部同色无需处理 */
    for (int i = 0; i < 256; i++) {
        cdf += hist[i];
        int v = (int)((cdf - mincdf) * 255.0 / denom + 0.5);
        if (v < 0) v = 0;
        if (v > 255) v = 255;
        lut[i] = (uint8_t)v;
    }
    for (int i = 0; i < n; i++) gray->data[i] = lut[gray->data[i]];
    return IP_OK;
}

IpStatus ip_histogram_save_ascii(const char *path, const uint64_t hist[256], int bar_h) {
    if (!path || !hist) return IP_ERR_NULL_PTR;
    if (bar_h <= 0) bar_h = 20;
    FILE *fp = fopen(path, "w");
    if (!fp) return IP_ERR_FILE_OPEN;
    uint64_t maxv = 0;
    for (int i = 0; i < 256; i++) if (hist[i] > maxv) maxv = hist[i];
    if (maxv == 0) maxv = 1;
    fprintf(fp, "Histogram (max=%llu)\n", (unsigned long long)maxv);
    for (int r = bar_h; r >= 1; r--) {
        for (int i = 0; i < 256; i++) {
            uint64_t val = (uint64_t)((double)hist[i] * bar_h / maxv + 0.5);
            fputc(val >= (uint64_t)r ? '#' : ' ', fp);
        }
        fputc('\n', fp);
    }
    /* 绘制0和255刻度 */
    for (int i = 0; i < 256; i++) {
        if (i == 0) fputc('0', fp);
        else if (i == 128) fputc('M', fp);
        else if (i == 255) fputc('F', fp);
        else fputc('-', fp);
    }
    fputc('\n', fp);
    fclose(fp);
    return IP_OK;
}
