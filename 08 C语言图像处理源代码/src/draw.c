/**
 * draw.c —— 基础图元绘制：像素点、Bresenham直线、矩形、中点圆算法
 */
#include "imageproc.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static inline void set_pixel(Image *img, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x<0 || x>=img->width || y<0 || y>=img->height) return;
    if (img->fmt == IP_FMT_RGB24) {
        IP_R(img,x,y)=r; IP_G(img,x,y)=g; IP_B(img,x,y)=b;
    } else {
        /* 灰度图：NTSC加权 */
        IP_GRAY(img,x,y) = (uint8_t)((r*30+g*59+b*11+50)/100);
    }
}

IpStatus ip_draw_pixel(Image *img, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (!img) return IP_ERR_NULL_PTR;
    set_pixel(img, x, y, r, g, b);
    return IP_OK;
}

/* Bresenham直线算法 */
IpStatus ip_draw_line(Image *img, int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b) {
    if (!img) return IP_ERR_NULL_PTR;
    int dx = abs(x2-x1), dy = abs(y2-y1);
    int sx = x1<x2 ? 1 : -1;
    int sy = y1<y2 ? 1 : -1;
    int err = dx - dy;
    while (1) {
        set_pixel(img, x1, y1, r, g, b);
        if (x1==x2 && y1==y2) break;
        int e2 = err*2;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
    return IP_OK;
}

IpStatus ip_draw_rect(Image *img, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, int filled) {
    if (!img) return IP_ERR_NULL_PTR;
    if (filled) {
        for (int j=y; j<y+h; j++)
            for (int i=x; i<x+w; i++)
                set_pixel(img, i, j, r, g, b);
    } else {
        ip_draw_line(img, x,   y,   x+w-1, y,     r,g,b);
        ip_draw_line(img, x+w-1,y,  x+w-1, y+h-1, r,g,b);
        ip_draw_line(img, x+w-1,y+h-1, x, y+h-1, r,g,b);
        ip_draw_line(img, x,   y+h-1, x,   y,     r,g,b);
    }
    return IP_OK;
}

/* 中点圆算法（8分对称） */
static void draw_circle_points(Image *img, int cx, int cy, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    set_pixel(img, cx+x, cy+y, r,g,b); set_pixel(img, cx+y, cy+x, r,g,b);
    set_pixel(img, cx-x, cy+y, r,g,b); set_pixel(img, cx+y, cy-x, r,g,b);
    set_pixel(img, cx+x, cy-y, r,g,b); set_pixel(img, cx-y, cy+x, r,g,b);
    set_pixel(img, cx-x, cy-y, r,g,b); set_pixel(img, cx-y, cy-x, r,g,b);
}
static void flood_horiz(Image *img, int y, int xl, int xr, uint8_t r, uint8_t g, uint8_t b) {
    for (int x=xl; x<=xr; x++) set_pixel(img, x, y, r, g, b);
}
IpStatus ip_draw_circle(Image *img, int cx, int cy, int rad, uint8_t r, uint8_t g, uint8_t b, int filled) {
    if (!img) return IP_ERR_NULL_PTR;
    if (filled) {
        /* 简化扫描线填充：按行画水平线段 */
        for (int dy=-rad; dy<=rad; dy++) {
            int dx = (int)sqrt((double)rad*rad - (double)dy*dy);
            flood_horiz(img, cy+dy, cx-dx, cx+dx, r, g, b);
        }
    } else {
        int x=0, y=rad, d=1-rad;
        draw_circle_points(img, cx, cy, x, y, r,g,b);
        while (x<y) {
            if (d<0) { d += 2*x+3; }
            else     { d += 2*(x-y)+5; y--; }
            x++;
            draw_circle_points(img, cx, cy, x, y, r,g,b);
        }
    }
    return IP_OK;
}
