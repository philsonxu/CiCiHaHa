/*
 * svg_graphics.c - TURBOC graphics.h 兼容SVG输出库实现
 * 纯C实现，输出标准SVG矢量图形
 */

#include "graphics.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ======================== 全局状态 ======================== */
svg_graphics_state g_svg;

/* BGI 16色调色板映射到标准网页颜色 */
static const char *bgi_palette[16] = {
    "#000000",  /* 0  BLACK      黑   */
    "#0000AA",  /* 1  BLUE       蓝   */
    "#00AA00",  /* 2  GREEN      绿   */
    "#00AAAA",  /* 3  CYAN       青   */
    "#AA0000",  /* 4  RED        红   */
    "#AA00AA",  /* 5  MAGENTA    紫   */
    "#AA5500",  /* 6  BROWN      棕   */
    "#AAAAAA",  /* 7  LIGHTGRAY  浅灰 */
    "#555555",  /* 8  DARKGRAY   深灰 */
    "#5555FF",  /* 9  LIGHTBLUE  亮蓝 */
    "#55FF55",  /* 10 LIGHTGREEN 亮绿 */
    "#55FFFF",  /* 11 LIGHTCYAN  亮青 */
    "#FF5555",  /* 12 LIGHTRED   亮红 */
    "#FF55FF",  /* 13 LIGHTMAGENTA亮紫 */
    "#FFFF55",  /* 14 YELLOW     黄   */
    "#FFFFFF"   /* 15 WHITE      白   */
};

/* ======================== 内部辅助函数 ======================== */

/* 将角度(度,0=东,逆时针)转换为SVG弧度(0=东,顺时针?SVG中y向下需修正) */
static double deg_to_svg_angle(double deg) {
    /* SVG坐标系y向下，数学上y向上，需要映射:
       BGI: 0度=向右(x+), 逆时针为正
       SVG arc: 角度0度指向3点钟方向(x+), 正角度为顺时针方向
       因此BGI角度转SVG角度直接用即可(sweep-flag控制方向) */
    return deg * M_PI / 180.0;
}

/* 根据线型和线宽生成stroke样式字符串 */
static void svg_write_stroke_style(FILE *fp) {
    const char *c = g_svg.palette[g_svg.fg_color];
    int w = g_svg.line.thickness;

    fprintf(fp, " stroke=\"%s\" stroke-width=\"%d\"", c, w);

    /* stroke-dasharray */
    switch (g_svg.line.linestyle) {
        case SOLID_LINE:
            break;
        case DOTTED_LINE:
            fprintf(fp, " stroke-dasharray=\"1,%d\"", w > 1 ? w+2 : 3);
            break;
        case CENTER_LINE:
            fprintf(fp, " stroke-dasharray=\"%d,%d,%d,%d\"",
                    6*w, 2*w, 2*w, 2*w);
            break;
        case DASHED_LINE:
            fprintf(fp, " stroke-dasharray=\"%d,%d\"", 6*w, 3*w);
            break;
        case USERBIT_LINE: {
            /* 根据16位upattern生成dash数组 */
            unsigned p = g_svg.line.upattern;
            int dash[32], n = 0, i, count = 0, cur = (p >> 15) & 1;
            for (i = 15; i >= 0; i--) {
                int bit = (p >> i) & 1;
                if (bit == cur) {
                    count++;
                } else {
                    dash[n++] = count;
                    cur = bit;
                    count = 1;
                }
            }
            dash[n++] = count;
            fprintf(fp, " stroke-dasharray=\"");
            for (i = 0; i < n; i++) {
                if (i > 0) fprintf(fp, ",");
                fprintf(fp, "%d", dash[i]);
            }
            fprintf(fp, "\"");
            break;
        }
    }

    /* 线端圆角 */
    fprintf(fp, " stroke-linecap=\"round\" stroke-linejoin=\"round\"");

    /* XOR模式通过opacity模拟 */
    if (g_svg.writemode == XOR_PUT) {
        fprintf(fp, " opacity=\"0.7\"");
    }
}

/* 生成fill属性 */
static void svg_write_fill_attr(FILE *fp, int fill_color, int use_pattern) {
    if (g_svg.fill.pattern == EMPTY_FILL) {
        fprintf(fp, " fill=\"none\"");
        return;
    }
    if (g_svg.fill.pattern == SOLID_FILL || use_pattern == 0) {
        fprintf(fp, " fill=\"%s\"", g_svg.palette[fill_color]);
    } else {
        fprintf(fp, " fill=\"url(#hatch%d)\"", g_svg.pattern_id - 1);
    }
}

/* 填充图案定义(写入defs) */
static void svg_define_hatch_pattern(FILE *fp, int pattern, int color, int id) {
    const char *c = g_svg.palette[color];
    fprintf(fp, "<pattern id=\"hatch%d\" patternUnits=\"userSpaceOnUse\" ", id);
    switch (pattern) {
        case LINE_FILL:
            fprintf(fp, "width=\"8\" height=\"8\"><path d=\"M0,0 L8,0\" stroke=\"%s\"/></pattern>", c);
            break;
        case LTSLASH_FILL:
            fprintf(fp, "width=\"8\" height=\"8\"><path d=\"M0,8 L8,0\" stroke=\"%s\" stroke-width=\"0.5\"/></pattern>", c);
            break;
        case SLASH_FILL:
            fprintf(fp, "width=\"8\" height=\"8\"><path d=\"M0,8 L8,0\" stroke=\"%s\"/></pattern>", c);
            break;
        case BKSLASH_FILL:
            fprintf(fp, "width=\"8\" height=\"8\"><path d=\"M0,0 L8,8\" stroke=\"%s\"/></pattern>", c);
            break;
        case LTBKSLASH_FILL:
            fprintf(fp, "width=\"8\" height=\"8\"><path d=\"M0,0 L8,8\" stroke=\"%s\" stroke-width=\"0.5\"/></pattern>", c);
            break;
        case HATCH_FILL:
            fprintf(fp, "width=\"8\" height=\"8\"><path d=\"M0,0 L8,0 M0,0 L0,8\" stroke=\"%s\"/></pattern>", c);
            break;
        case XHATCH_FILL:
            fprintf(fp, "width=\"8\" height=\"8\"><path d=\"M0,8 L8,0 M0,0 L8,8\" stroke=\"%s\"/></pattern>", c);
            break;
        case INTERLEAVE_FILL:
            fprintf(fp, "width=\"8\" height=\"8\"><path d=\"M0,0 L8,0 M4,0 L4,8\" stroke=\"%s\"/></pattern>", c);
            break;
        case WIDE_DOT_FILL:
            fprintf(fp, "width=\"8\" height=\"8\"><circle cx=\"2\" cy=\"2\" r=\"1\" fill=\"%s\"/></pattern>", c);
            break;
        case CLOSE_DOT_FILL:
            fprintf(fp, "width=\"4\" height=\"4\"><circle cx=\"1\" cy=\"1\" r=\"0.8\" fill=\"%s\"/></pattern>", c);
            break;
        default:
            fprintf(fp, "width=\"8\" height=\"8\" fill=\"%s\"/></pattern>", c);
            break;
    }
}

/* 转换视口坐标 -> 全局坐标 */
static int vx(int x) { return g_svg.viewport.left + x; }
static int vy(int y) { return g_svg.viewport.top + y; }

/* ======================== 公共API实现 ======================== */

void svg_write_header(int width, int height) {
    fprintf(g_svg.fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(g_svg.fp, "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            "width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\">\n",
            width, height, width, height);
    fprintf(g_svg.fp, "<defs></defs>\n");
    /* 背景矩形 */
    fprintf(g_svg.fp, "<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" fill=\"%s\"/>\n",
            width, height, g_svg.palette[g_svg.bk_color]);
}

void svg_write_footer(void) {
    fprintf(g_svg.fp, "</svg>\n");
}

void initgraph_svg(const char *filename, int width, int height) {
    memset(&g_svg, 0, sizeof(g_svg));
    g_svg.fp = fopen(filename, "w");
    if (!g_svg.fp) {
        g_svg.errorcode = grFileNotFound;
        return;
    }
    g_svg.width = width;
    g_svg.height = height;
    g_svg.init = 1;
    g_svg.errorcode = grOk;

    /* CP初始位置 */
    g_svg.cp_x = 0;
    g_svg.cp_y = 0;

    /* 视口初始值 */
    g_svg.viewport.left = 0;
    g_svg.viewport.top = 0;
    g_svg.viewport.right = width - 1;
    g_svg.viewport.bottom = height - 1;
    g_svg.viewport.clipflag = 1;

    /* 默认颜色 */
    g_svg.fg_color = WHITE;
    g_svg.bk_color = BLACK;
    memcpy((void*)g_svg.palette, bgi_palette, sizeof(bgi_palette));

    /* 默认线型 */
    g_svg.line.linestyle = SOLID_LINE;
    g_svg.line.upattern = 0xFFFF;
    g_svg.line.thickness = NORM_WIDTH;

    /* 默认填充 */
    g_svg.fill.pattern = SOLID_FILL;
    g_svg.fill.color = WHITE;

    /* 默认文本 */
    g_svg.text.font = DEFAULT_FONT;
    g_svg.text.direction = HORIZ_DIR;
    g_svg.text.charsize = 1;
    g_svg.text.horiz = LEFT_TEXT;
    g_svg.text.vert = TOP_TEXT;
    g_svg.text_size = 12;

    /* 其他默认值 */
    g_svg.writemode = COPY_PUT;
    g_svg.xasp = 10000;
    g_svg.yasp = 10000;
    g_svg.pattern_id = 0;

    svg_write_header(width, height);
}

void initgraph(int *gdriver, int *gmode, const char *pathtodriver) {
    (void)pathtodriver;
    int w = 640, h = 480;
    if (*gdriver == DETECT) {
        *gdriver = VGA;
        *gmode = VGAHI;
    }
    /* 根据模式决定分辨率 */
    if (*gdriver == VGA && *gmode == VGAHI) { w = 640; h = 480; }
    else if (*gdriver == VGA && *gmode == VGAMED) { w = 640; h = 350; }
    else if (*gdriver == VGA && *gmode == VGALO) { w = 640; h = 200; }
    else if (*gdriver == EGA && *gmode == EGAHI) { w = 640; h = 350; }
    else if (*gdriver == CGA && *gmode == CGAHI) { w = 640; h = 200; }

    initgraph_svg("output.svg", w, h);
}

void closegraph(void) {
    if (g_svg.fp) {
        svg_write_footer();
        fclose(g_svg.fp);
        g_svg.fp = NULL;
    }
    g_svg.init = 0;

    system("start output.svg");
}

void detectgraph(int *gdriver, int *gmode) {
    *gdriver = VGA;
    *gmode = VGAHI;
}

int graphresult(void) {
    int e = g_svg.errorcode;
    g_svg.errorcode = grOk;
    return e;
}

const char *grapherrormsg(int errcode) {
    switch (errcode) {
        case grOk: return "No error";
        case grNoInitGraph: return "(BGI) graphics not installed (use initgraph)";
        case grNotDetected: return "Graphics hardware not detected";
        case grFileNotFound: return "Device driver file not found";
        case grInvalidDriver: return "Invalid device driver file";
        case grNoLoadMem: return "Not enough memory to load driver";
        case grNoScanMem: return "Out of memory in scan fill";
        case grNoFloodMem: return "Out of memory in flood fill";
        case grFontNotFound: return "Font file not found";
        case grNoFontMem: return "Not enough memory to load font";
        case grInvalidMode: return "Invalid graphics mode for selected driver";
        case grError: return "Graphics error";
        case grIOerror: return "Graphics I/O error";
        case grInvalidFont: return "Invalid font file";
        case grInvalidFontNum: return "Invalid font number";
        default: return "Unknown error";
    }
}

void setgraphmode(int mode) {
    (void)mode;
    if (g_svg.fp) { clearviewport(); }
}

void restorecrtmode(void) {
    /* 简化处理：什么也不做 */
}

void cleardevice(void) {
    /* 重画背景 */
    fprintf(g_svg.fp, "<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" fill=\"%s\"/>\n",
            g_svg.width, g_svg.height, g_svg.palette[g_svg.bk_color]);
    g_svg.cp_x = 0;
    g_svg.cp_y = 0;
}

void clearviewport(void) {
    /* 用背景色填充视口区域 */
    int l = g_svg.viewport.left, t = g_svg.viewport.top;
    int w = g_svg.viewport.right - l + 1;
    int h = g_svg.viewport.bottom - t + 1;
    fprintf(g_svg.fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"%s\"/>\n",
            l, t, w, h, g_svg.palette[g_svg.bk_color]);
    g_svg.cp_x = 0;
    g_svg.cp_y = 0;
}

/********* 视口与坐标 *********/
void setviewport(int left, int top, int right, int bottom, int clip) {
    g_svg.viewport.left = left;
    g_svg.viewport.top = top;
    g_svg.viewport.right = right;
    g_svg.viewport.bottom = bottom;
    g_svg.viewport.clipflag = clip;

    /* 视口变换：添加g分组和clipPath */
    if (clip) {
        static int clip_id = 0;
        fprintf(g_svg.fp, "<defs><clipPath id=\"clip%d\"><rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\"/>"
                "</clipPath></defs>\n", clip_id, left, top, right - left + 1, bottom - top + 1);
        fprintf(g_svg.fp, "<g clip-path=\"url(#clip%d)\">\n", clip_id);
        clip_id++;
    } else {
        fprintf(g_svg.fp, "<g transform=\"translate(%d,%d)\">\n", left, top);
    }
    g_svg.cp_x = 0;
    g_svg.cp_y = 0;
}

void getviewsettings(struct viewporttype *viewport) {
    if (viewport) *viewport = g_svg.viewport;
}

int getmaxx(void) { return g_svg.width - 1; }
int getmaxy(void) { return g_svg.height - 1; }
int getx(void) { return g_svg.cp_x; }
int gety(void) { return g_svg.cp_y; }

void moveto(int x, int y) {
    g_svg.cp_x = x;
    g_svg.cp_y = y;
}

void moverel(int dx, int dy) {
    g_svg.cp_x += dx;
    g_svg.cp_y += dy;
}

/********* 颜色 *********/
const char *svg_color(int idx) {
    if (idx < 0 || idx > 15) idx = WHITE;
    return g_svg.palette[idx];
}

void setcolor(int color) { g_svg.fg_color = color & 0x0F; }
void setbkcolor(int color) { g_svg.bk_color = color & 0x0F; }
int getcolor(void) { return g_svg.fg_color; }
int getbkcolor(void) { return g_svg.bk_color; }
int getmaxcolor(void) { return 15; }

void setpalette(int colornum, int color) {
    (void)color;
    if (colornum >= 0 && colornum <= 15) {
        /* 标准实现修改调色板，此处简化忽略 */
    }
}

void setallpalette(struct palettetype *palette) {
    (void)palette;
}

/********* 像素 *********/
void putpixel(int x, int y, int color) {
    int ax = vx(x), ay = vy(y);
    fprintf(g_svg.fp, "<rect x=\"%d\" y=\"%d\" width=\"1\" height=\"1\" fill=\"%s\"/>\n",
            ax, ay, g_svg.palette[color & 0x0F]);
}

int getpixel(int x, int y) {
    (void)x; (void)y;
    return g_svg.fg_color; /* 简化实现 */
}

/********* 直线 *********/
void line(int x1, int y1, int x2, int y2) {
    int ax1 = vx(x1), ay1 = vy(y1);
    int ax2 = vx(x2), ay2 = vy(y2);
    fprintf(g_svg.fp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\"", ax1, ay1, ax2, ay2);
    fprintf(g_svg.fp, " fill=\"none\"");
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");
}

void lineto(int x, int y) {
    int ax1 = vx(g_svg.cp_x), ay1 = vy(g_svg.cp_y);
    int ax2 = vx(x), ay2 = vy(y);
    fprintf(g_svg.fp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\"", ax1, ay1, ax2, ay2);
    fprintf(g_svg.fp, " fill=\"none\"");
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");
    g_svg.cp_x = x;
    g_svg.cp_y = y;
}

void linerel(int dx, int dy) {
    lineto(g_svg.cp_x + dx, g_svg.cp_y + dy);
}

void setlinestyle(int linestyle, unsigned upattern, int thickness) {
    g_svg.line.linestyle = linestyle;
    g_svg.line.upattern = upattern;
    g_svg.line.thickness = thickness;
}

void getlinesettings(struct linesettingstype *lineinfo) {
    if (lineinfo) *lineinfo = g_svg.line;
}

void setwritemode(int mode) { g_svg.writemode = mode; }

/********* 矩形与多边形 *********/
void rectangle(int left, int top, int right, int bottom) {
    int al = vx(left), at = vy(top);
    int w = right - left, h = bottom - top;
    fprintf(g_svg.fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"none\"",
            al, at, w, h);
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");
}

void bar(int left, int top, int right, int bottom) {
    int al = vx(left), at = vy(top);
    int w = right - left + 1, h = bottom - top + 1;
    int need_pattern = (g_svg.fill.pattern != SOLID_FILL && g_svg.fill.pattern != EMPTY_FILL);

    if (need_pattern) {
        g_svg.pattern_id++;
        svg_define_hatch_pattern(g_svg.fp, g_svg.fill.pattern, g_svg.fill.color, g_svg.pattern_id - 1);
    }

    fprintf(g_svg.fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\"", al, at, w, h);
    svg_write_fill_attr(g_svg.fp, g_svg.fill.color, need_pattern);
    fprintf(g_svg.fp, "/>\n");
}

void bar3d(int left, int top, int right, int bottom, int depth, int topflag) {
    /* 正面 */
    bar(left, top, right, bottom);

    /* 顶面 */
    if (topflag) {
        int al = vx(left), at = vy(top);
        int ar = vx(right), ab = vy(bottom);
        fprintf(g_svg.fp, "<polygon points=\"%d,%d %d,%d %d,%d %d,%d\"",
                al, at,
                al + depth, at - depth,
                ar + depth, at - depth,
                ar, at);
        fprintf(g_svg.fp, " fill=\"%s\"", g_svg.palette[g_svg.fill.color]);
        fprintf(g_svg.fp, " stroke=\"%s\" stroke-width=\"1\"/>\n", g_svg.palette[g_svg.fg_color]);
        /* 右侧面 */
        fprintf(g_svg.fp, "<polygon points=\"%d,%d %d,%d %d,%d %d,%d\"",
                ar, at,
                ar + depth, at - depth,
                ar + depth, ab - depth,
                ar, ab);
        fprintf(g_svg.fp, " fill=\"%s\"", g_svg.palette[g_svg.bk_color != BLACK ? g_svg.bk_color : DARKGRAY]);
        fprintf(g_svg.fp, " stroke=\"%s\" stroke-width=\"1\"/>\n", g_svg.palette[g_svg.fg_color]);
    }
}

void drawpoly(int numpoints, const int *polypoints) {
    int i;
    fprintf(g_svg.fp, "<polyline points=\"");
    for (i = 0; i < numpoints; i++) {
        if (i > 0) fprintf(g_svg.fp, " ");
        fprintf(g_svg.fp, "%d,%d", vx(polypoints[2*i]), vy(polypoints[2*i+1]));
    }
    fprintf(g_svg.fp, "\" fill=\"none\"");
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");
}

void fillpoly(int numpoints, const int *polypoints) {
    int i;
    int need_pattern = (g_svg.fill.pattern != SOLID_FILL && g_svg.fill.pattern != EMPTY_FILL);
    if (need_pattern) {
        g_svg.pattern_id++;
        svg_define_hatch_pattern(g_svg.fp, g_svg.fill.pattern, g_svg.fill.color, g_svg.pattern_id - 1);
    }

    fprintf(g_svg.fp, "<polygon points=\"");
    for (i = 0; i < numpoints; i++) {
        if (i > 0) fprintf(g_svg.fp, " ");
        fprintf(g_svg.fp, "%d,%d", vx(polypoints[2*i]), vy(polypoints[2*i+1]));
    }
    fprintf(g_svg.fp, "\"");
    svg_write_fill_attr(g_svg.fp, g_svg.fill.color, need_pattern);
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");
}

/********* 圆弧曲线 *********/
void getaspectratio(int *xasp, int *yasp) {
    *xasp = g_svg.xasp;
    *yasp = g_svg.yasp;
}

void circle(int x, int y, int radius) {
    int ax = vx(x), ay = vy(y);
    fprintf(g_svg.fp, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"none\"", ax, ay, radius);
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");

    /* 记录arc坐标(整圆无起止) */
    g_svg.arc.x = x;
    g_svg.arc.y = y;
    g_svg.arc.xstart = x + radius;
    g_svg.arc.ystart = y;
    g_svg.arc.xend = x + radius;
    g_svg.arc.yend = y;
}

void arc(int x, int y, int stangle, int endangle, int radius) {
    int ax = vx(x), ay = vy(y);
    double a1 = deg_to_svg_angle(stangle);
    double a2 = deg_to_svg_angle(endangle);

    /* BGI逆时针，SVG sweep-flag: 1=顺时针, 0=逆时针(y轴向下) */
    int x1 = ax + (int)(radius * cos(a1));
    int y1 = ay + (int)(radius * sin(a1));
    int x2 = ax + (int)(radius * cos(a2));
    int y2 = ay + (int)(radius * sin(a2));
    int large_arc = (endangle - stangle) > 180 ? 1 : 0;
    int sweep = 0; /* 逆时针 */

    /* 修正y方向: sin在SVG(y向下)要取负值? 实际上BGI y向下，数学y向上:
       屏幕坐标y向下,故 sin(a1) 应取正(向下). 对的，无需取反 */

    fprintf(g_svg.fp, "<path d=\"M %d,%d A %d,%d 0 %d,%d %d,%d\" fill=\"none\"",
            x1, y1, radius, radius, large_arc, sweep, x2, y2);
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");

    g_svg.arc.x = x;
    g_svg.arc.y = y;
    g_svg.arc.xstart = x1 - g_svg.viewport.left;
    g_svg.arc.ystart = y1 - g_svg.viewport.top;
    g_svg.arc.xend = x2 - g_svg.viewport.left;
    g_svg.arc.yend = y2 - g_svg.viewport.top;
}

void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius) {
    int ax = vx(x), ay = vy(y);
    if (stangle == 0 && endangle == 360) {
        fprintf(g_svg.fp, "<ellipse cx=\"%d\" cy=\"%d\" rx=\"%d\" ry=\"%d\" fill=\"none\"",
                ax, ay, xradius, yradius);
        svg_write_stroke_style(g_svg.fp);
        fprintf(g_svg.fp, "/>\n");
    } else {
        double a1 = deg_to_svg_angle(stangle);
        double a2 = deg_to_svg_angle(endangle);
        int x1 = ax + (int)(xradius * cos(a1));
        int y1 = ay + (int)(yradius * sin(a1));
        int x2 = ax + (int)(xradius * cos(a2));
        int y2 = ay + (int)(yradius * sin(a2));
        int large_arc = (endangle - stangle) > 180 ? 1 : 0;
        fprintf(g_svg.fp, "<path d=\"M %d,%d A %d,%d 0 %d,0 %d,%d\" fill=\"none\"",
                x1, y1, xradius, yradius, large_arc, x2, y2);
        svg_write_stroke_style(g_svg.fp);
        fprintf(g_svg.fp, "/>\n");
    }
}

void fillellipse(int x, int y, int xradius, int yradius) {
    int ax = vx(x), ay = vy(y);
    int need_pattern = (g_svg.fill.pattern != SOLID_FILL && g_svg.fill.pattern != EMPTY_FILL);
    if (need_pattern) {
        g_svg.pattern_id++;
        svg_define_hatch_pattern(g_svg.fp, g_svg.fill.pattern, g_svg.fill.color, g_svg.pattern_id - 1);
    }
    fprintf(g_svg.fp, "<ellipse cx=\"%d\" cy=\"%d\" rx=\"%d\" ry=\"%d\"", ax, ay, xradius, yradius);
    svg_write_fill_attr(g_svg.fp, g_svg.fill.color, need_pattern);
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");
}

void pieslice(int x, int y, int stangle, int endangle, int radius) {
    int ax = vx(x), ay = vy(y);
    double a1 = deg_to_svg_angle(stangle);
    double a2 = deg_to_svg_angle(endangle);
    int x1 = ax + (int)(radius * cos(a1));
    int y1 = ay + (int)(radius * sin(a1));
    int x2 = ax + (int)(radius * cos(a2));
    int y2 = ay + (int)(radius * sin(a2));
    int large_arc = (endangle - stangle) > 180 ? 1 : 0;

    int need_pattern = (g_svg.fill.pattern != SOLID_FILL && g_svg.fill.pattern != EMPTY_FILL);
    if (need_pattern) {
        g_svg.pattern_id++;
        svg_define_hatch_pattern(g_svg.fp, g_svg.fill.pattern, g_svg.fill.color, g_svg.pattern_id - 1);
    }

    fprintf(g_svg.fp, "<path d=\"M %d,%d L %d,%d A %d,%d 0 %d,0 %d,%d Z\"",
            ax, ay, x1, y1, radius, radius, large_arc, x2, y2);
    svg_write_fill_attr(g_svg.fp, g_svg.fill.color, need_pattern);
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");

    g_svg.arc.x = x;
    g_svg.arc.y = y;
    g_svg.arc.xstart = x1 - g_svg.viewport.left;
    g_svg.arc.ystart = y1 - g_svg.viewport.top;
    g_svg.arc.xend = x2 - g_svg.viewport.left;
    g_svg.arc.yend = y2 - g_svg.viewport.top;
}

void sector(int x, int y, int stangle, int endangle, int xradius, int yradius) {
    int ax = vx(x), ay = vy(y);
    double a1 = deg_to_svg_angle(stangle);
    double a2 = deg_to_svg_angle(endangle);
    int x1 = ax + (int)(xradius * cos(a1));
    int y1 = ay + (int)(yradius * sin(a1));
    int x2 = ax + (int)(xradius * cos(a2));
    int y2 = ay + (int)(yradius * sin(a2));
    int large_arc = (endangle - stangle) > 180 ? 1 : 0;

    int need_pattern = (g_svg.fill.pattern != SOLID_FILL && g_svg.fill.pattern != EMPTY_FILL);
    if (need_pattern) {
        g_svg.pattern_id++;
        svg_define_hatch_pattern(g_svg.fp, g_svg.fill.pattern, g_svg.fill.color, g_svg.pattern_id - 1);
    }

    fprintf(g_svg.fp, "<path d=\"M %d,%d L %d,%d A %d,%d 0 %d,0 %d,%d Z\"",
            ax, ay, x1, y1, xradius, yradius, large_arc, x2, y2);
    svg_write_fill_attr(g_svg.fp, g_svg.fill.color, need_pattern);
    svg_write_stroke_style(g_svg.fp);
    fprintf(g_svg.fp, "/>\n");
}

void getarccoords(struct arccoordstype *arccoords) {
    if (arccoords) *arccoords = g_svg.arc;
}

/********* 填充 *********/
void setfillstyle(int pattern, int color) {
    g_svg.fill.pattern = pattern;
    g_svg.fill.color = color;
}

void setfillpattern(const char *upattern, int color) {
    g_svg.fill.pattern = USER_FILL;
    g_svg.fill.color = color;
    if (upattern) memcpy(g_svg.user_pattern, upattern, 8);
}

void floodfill(int x, int y, int border) {
    (void)x; (void)y; (void)border;
    /* SVG中floodfill难以精确模拟，此处简化为不操作 */
}

void getfillsettings(struct fillsettingstype *fillinfo) {
    if (fillinfo) *fillinfo = g_svg.fill;
}

void getfillpattern(char *pattern) {
    if (pattern) memcpy(pattern, g_svg.user_pattern, 8);
}

/********* 文本 *********/
static const char *font_family(int font) {
    switch (font) {
        case DEFAULT_FONT:    return "monospace";
        case TRIPLEX_FONT:    return "serif";
        case SMALL_FONT:      return "sans-serif";
        case SANS_SERIF_FONT: return "sans-serif";
        case GOTHIC_FONT:     return "serif";
        default:              return "monospace";
    }
}

void outtextxy(int x, int y, const char *textstring) {
    int ax = vx(x), ay = vy(y);
    int font_size = 8 + g_svg.text.charsize * 4;
    const char *anchor = "start";
    const char *baseline = "alphabetic";
    int font_weight = (g_svg.text.font == GOTHIC_FONT) ? 700 : 400;

    switch (g_svg.text.horiz) {
        case LEFT_TEXT:   anchor = "start"; break;
        case CENTER_TEXT: anchor = "middle"; break;
        case RIGHT_TEXT:  anchor = "end"; break;
    }
    switch (g_svg.text.vert) {
        case TOP_TEXT:    baseline = "text-before-edge"; break;
        case CENTER_TEXT: baseline = "middle"; break;
        case BOTTOM_TEXT: baseline = "text-after-edge"; break;
    }

    if (g_svg.text.direction == HORIZ_DIR) {
        fprintf(g_svg.fp, "<text x=\"%d\" y=\"%d\" font-family=\"%s\" font-size=\"%d\" "
                "font-weight=\"%d\" fill=\"%s\" text-anchor=\"%s\" dominant-baseline=\"%s\">%s</text>\n",
                ax, ay, font_family(g_svg.text.font), font_size, font_weight,
                g_svg.palette[g_svg.fg_color], anchor, baseline, textstring);
    } else {
        /* 垂直文字: 旋转90度 */
        fprintf(g_svg.fp, "<text x=\"%d\" y=\"%d\" font-family=\"%s\" font-size=\"%d\" "
                "font-weight=\"%d\" fill=\"%s\" text-anchor=\"%s\" dominant-baseline=\"%s\" "
                "transform=\"rotate(-90 %d,%d)\">%s</text>\n",
                ax, ay, font_family(g_svg.text.font), font_size, font_weight,
                g_svg.palette[g_svg.fg_color], anchor, baseline, ax, ay, textstring);
    }
}

void outtext(const char *textstring) {
    outtextxy(g_svg.cp_x, g_svg.cp_y, textstring);
    /* 粗略更新CP */
    g_svg.cp_x += (int)(strlen(textstring) * (8 + g_svg.text.charsize * 4) * 0.6);
}

void settextstyle(int font, int direction, int charsize) {
    g_svg.text.font = font;
    g_svg.text.direction = direction;
    g_svg.text.charsize = charsize;
}

void settextjustify(int horiz, int vert) {
    g_svg.text.horiz = horiz;
    g_svg.text.vert = vert;
}

int textwidth(const char *textstring) {
    if (!textstring) return 0;
    return (int)(strlen(textstring) * (8 + g_svg.text.charsize * 4) * 0.6);
}

int textheight(const char *textstring) {
    (void)textstring;
    return 8 + g_svg.text.charsize * 4;
}

/********* 图像(简化存根) *********/
unsigned imagesize(int left, int top, int right, int bottom) {
    int w = right - left + 1, h = bottom - top + 1;
    return (unsigned)(4 + w * h * 4);
}

void getimage(int left, int top, int right, int bottom, void *bitmap) {
    (void)left; (void)top; (void)right; (void)bottom;
    if (bitmap) memset(bitmap, 0, imagesize(left,top,right,bottom));
}

void putimage(int left, int top, void *bitmap, int op) {
    (void)left; (void)top; (void)bitmap; (void)op;
    /* 简化实现：SVG中图像处理复杂，此处不实现 */
}

int random(int n)
{
    return rand() % n;
}

// 动画的帧序号
int frame = 0;
// 动画帧的开始时间（秒）
float frame_start = 0.0;
// 动画帧的间隔时间（秒）
float frame_duration = 0.08;

void animate()
{
    frame = 1;
    frame_start = 0.0;
    frame_duration = 0.08;
}

void framebegin()
{
    fprintf(g_svg.fp, "<g id=\"f%d\" visibility=\"hidden\">\n", frame);
    fprintf(g_svg.fp, "<set attributeName=\"visibility\" to=\"visible\" begin=\"%.3fs\" dur=\"%.3fs\" fill=\"remove\"/>\n", frame_start, frame_duration);
}

void frameend()
{
    fprintf(g_svg.fp, "</g>\n");
    frame++;
    frame_start += frame_duration;
}
