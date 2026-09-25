/**
 * morphology.c —— 阈值二值化、Otsu自动阈值、形态学腐蚀/膨胀/开/闭运算
 */
#include "imageproc.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

IpStatus ip_threshold(Image *gray, uint8_t t, uint8_t maxv) {
    if (!gray) return IP_ERR_NULL_PTR;
    if (gray->fmt != IP_FMT_GRAY8) return IP_ERR_INVALID_FMT;
    int n = gray->width * gray->height;
    for (int i = 0; i < n; i++) {
        gray->data[i] = gray->data[i] > t ? maxv : 0;
    }
    return IP_OK;
}

IpStatus ip_threshold_otsu(const Image *gray, uint8_t *best_t) {
    if (!gray || !best_t) return IP_ERR_NULL_PTR;
    if (gray->fmt != IP_FMT_GRAY8) return IP_ERR_INVALID_FMT;
    uint64_t hist[256] = {0};
    int n = gray->width * gray->height;
    for (int i=0; i<n; i++) hist[gray->data[i]]++;

    double sum_all = 0;
    for (int i=0; i<256; i++) sum_all += i * hist[i];
    double w0=0, sum0=0, maxvar=0;
    uint8_t best = 0;
    for (int t=0; t<256; t++) {
        w0 += hist[t];
        if (w0 == 0) continue;
        double w1 = n - w0;
        if (w1 == 0) break;
        sum0 += t * hist[t];
        double m0 = sum0 / w0;
        double m1 = (sum_all - sum0) / w1;
        double var = w0 * w1 * (m0-m1)*(m0-m1);
        if (var > maxvar) { maxvar = var; best = (uint8_t)t; }
    }
    *best_t = best;
    return IP_OK;
}

/* ========== 形态学操作（k×k矩形结构元） ========== */
static IpStatus morph_op(const Image *bin, int ksize, Image **dst, int is_dilate) {
    if (!bin || !dst) return IP_ERR_NULL_PTR;
    if (bin->fmt != IP_FMT_GRAY8) return IP_ERR_INVALID_FMT;
    if (ksize < 1) return IP_ERR_OUT_OF_RANGE;
    if (ksize%2==0) ksize++;
    int w = bin->width, h = bin->height, half = ksize/2;
    Image *d; IpStatus s = ip_image_create(w, h, IP_FMT_GRAY8, &d);
    if (s != IP_OK) return s;
    /* 腐蚀：邻域有黑则黑；膨胀：邻域有白则白 */
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            uint8_t res = is_dilate ? 0 : 255;
            for (int ky=-half; ky<=half; ky++) {
                for (int kx=-half; kx<=half; kx++) {
                    int yy=y+ky, xx=x+kx;
                    if (yy<0)yy=0;
                    if (yy>=h)yy=h-1;
                    if (xx<0)xx=0;
                    if (xx>=w)xx=w-1;
                    uint8_t v = bin->data[(size_t)yy*w+xx];
                    if (is_dilate) { if (v==255) res=255; }
                    else           { if (v==0)   res=0;   }
                }
            }
            d->data[(size_t)y*w+x] = res;
        }
    }
    *dst = d; return IP_OK;
}

IpStatus ip_morph_erode (const Image *bin, int ksize, Image **dst) { return morph_op(bin, ksize, dst, 0); }
IpStatus ip_morph_dilate(const Image *bin, int ksize, Image **dst) { return morph_op(bin, ksize, dst, 1); }

IpStatus ip_morph_open(const Image *bin, int ksize, Image **dst) {
    /* 先腐蚀后膨胀：去小噪点 */
    Image *tmp = NULL;
    IpStatus s = ip_morph_erode(bin, ksize, &tmp);
    if (s != IP_OK) return s;
    s = ip_morph_dilate(tmp, ksize, dst);
    ip_image_release(&tmp);
    return s;
}
IpStatus ip_morph_close(const Image *bin, int ksize, Image **dst) {
    /* 先膨胀后腐蚀：填小黑洞 */
    Image *tmp = NULL;
    IpStatus s = ip_morph_dilate(bin, ksize, &tmp);
    if (s != IP_OK) return s;
    s = ip_morph_erode(tmp, ksize, dst);
    ip_image_release(&tmp);
    return s;
}
