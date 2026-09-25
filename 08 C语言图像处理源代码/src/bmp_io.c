/**
 * bmp_io.c —— BMP 24位真彩色读写
 *
 * BMP文件结构：
 *   BITMAPFILEHEADER (14字节) + BITMAPINFOHEADER (40字节) + 像素数据
 *   像素行需4字节对齐（每行字节数补齐为4的倍数），行序为从底到顶。
 *   像素按 B,G,R 顺序存储。
 */
#include "imageproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
    uint8_t  bfType[2];   /* "BM" */
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;   /* 像素数据偏移 = 54 */
} BmpFileHeader;

typedef struct {
    uint32_t biSize;          /* 40 */
    int32_t  biWidth;
    int32_t  biHeight;        /* 正数=从底到顶 */
    uint16_t biPlanes;        /* 1 */
    uint16_t biBitCount;      /* 24 */
    uint32_t biCompression;   /* 0=BI_RGB */
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BmpInfoHeader;
#pragma pack(pop)

IpStatus ip_bmp_load(const char *path, Image **out) {
    if (!path || !out) return IP_ERR_NULL_PTR;
    FILE *fp = fopen(path, "rb");
    if (!fp) return IP_ERR_FILE_OPEN;

    BmpFileHeader fh;
    BmpInfoHeader ih;
    if (fread(&fh, sizeof(fh), 1, fp) != 1) { fclose(fp); return IP_ERR_CORRUPTED; }
    if (fh.bfType[0] != 'B' || fh.bfType[1] != 'M') { fclose(fp); return IP_ERR_INVALID_FMT; }
    if (fread(&ih, sizeof(ih), 1, fp) != 1) { fclose(fp); return IP_ERR_CORRUPTED; }
    if (ih.biBitCount != 24 || ih.biCompression != 0) { fclose(fp); return IP_ERR_INVALID_FMT; }
    if (ih.biWidth <= 0 || ih.biHeight == 0) { fclose(fp); return IP_ERR_CORRUPTED; }

    int w = ih.biWidth;
    int top_down = 0;
    int h = ih.biHeight;
    if (h < 0) { h = -h; top_down = 1; } /* 负值=从上到下 */

    int row_bytes = w * 3;
    int padding = (4 - (row_bytes % 4)) % 4;
    int padded_row = row_bytes + padding;

    Image *img = NULL;
    IpStatus s = ip_image_create(w, h, IP_FMT_RGB24, &img);
    if (s != IP_OK) { fclose(fp); return s; }

    /* BMP内部行序：从底到顶（除非top-down负值） */
    uint8_t *rowbuf = (uint8_t*)malloc(padded_row);
    if (!rowbuf) { ip_image_release(&img); fclose(fp); return IP_ERR_ALLOC; }

    for (int i = 0; i < h; i++) {
        int y = top_down ? i : (h - 1 - i);
        if (fread(rowbuf, 1, padded_row, fp) != (size_t)padded_row) {
            free(rowbuf); ip_image_release(&img); fclose(fp); return IP_ERR_CORRUPTED;
        }
        uint8_t *dst = img->data + (size_t)y * w * 3;
        /* BMP文件中是BGR序，我们内部用RGB，读入时交换R↔B */
        memcpy(dst, rowbuf, (size_t)row_bytes);
        for (int k = 0; k < w; k++) {
            uint8_t t = dst[k*3+0];
            dst[k*3+0] = dst[k*3+2];
            dst[k*3+2] = t;
        }
    }
    free(rowbuf);
    fclose(fp);
    *out = img;
    return IP_OK;
}

IpStatus ip_bmp_save(const char *path, const Image *img) {
    if (!path || !img) return IP_ERR_NULL_PTR;
    if (img->fmt != IP_FMT_RGB24) return IP_ERR_INVALID_FMT;

    FILE *fp = fopen(path, "wb");
    if (!fp) return IP_ERR_FILE_OPEN;

    int w = img->width, h = img->height;
    int row_bytes = w * 3;
    int padding = (4 - (row_bytes % 4)) % 4;
    int padded_row = row_bytes + padding;
    uint32_t img_size = (uint32_t)padded_row * h;

    BmpFileHeader fh;
    memset(&fh, 0, sizeof(fh));
    fh.bfType[0] = 'B'; fh.bfType[1] = 'M';
    fh.bfOffBits = 54;
    fh.bfSize = fh.bfOffBits + img_size;

    BmpInfoHeader ih;
    memset(&ih, 0, sizeof(ih));
    ih.biSize = 40;
    ih.biWidth = w;
    ih.biHeight = h;        /* 正数=从底到顶 */
    ih.biPlanes = 1;
    ih.biBitCount = 24;
    ih.biCompression = 0;
    ih.biSizeImage = img_size;

    fwrite(&fh, sizeof(fh), 1, fp);
    fwrite(&ih, sizeof(ih), 1, fp);

    uint8_t pad[4] = {0,0,0,0};
    uint8_t *rowbuf = (uint8_t*)malloc((size_t)row_bytes);
    if (!rowbuf) { fclose(fp); return IP_ERR_ALLOC; }
    /* 写出顺序：从底到顶，每行RGB→BGR */
    for (int y = h - 1; y >= 0; y--) {
        const uint8_t *src = img->data + (size_t)y * w * 3;
        memcpy(rowbuf, src, (size_t)row_bytes);
        for (int k = 0; k < w; k++) {
            uint8_t t = rowbuf[k*3+0];
            rowbuf[k*3+0] = rowbuf[k*3+2];
            rowbuf[k*3+2] = t;
        }
        fwrite(rowbuf, 1, (size_t)row_bytes, fp);
        if (padding > 0) fwrite(pad, 1, (size_t)padding, fp);
    }
    free(rowbuf);
    fclose(fp);
    return IP_OK;
}
