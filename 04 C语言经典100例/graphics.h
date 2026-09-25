/*
 * svg_graphics.h - TURBOC graphics.h 兼容的SVG输出库
 * 纯C实现，将BGI图形调用输出为标准SVG矢量图文件
 *
 * ============================================================
 *  TURBOC graphics.h 函数  vs  SVG绘图元素  一一对比表
 * ============================================================
 *
 * 一、图形系统初始化与关闭
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * initgraph()           <svg>根元素，设置width/height/viewBox，创建XML文档
 * closegraph()          输出闭合标签</svg>，关闭文件，资源释放
 * detectgraph()         自动选择默认分辨率640x480(VGAHI)
 * graphresult()         返回错误码(grOk/grNoInitGraph等)
 * grapherrormsg()       返回错误描述字符串
 * setgraphmode()        设置绘图模式，重置画布大小
 * restorecrtmode()      重置为文本模式(此处仅清理状态)
 * cleardevice()         清除画布，添加背景rect填充
 * clearviewport()       清除当前视口区域
 *
 * 二、视口与坐标函数
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * setviewport()         建立<g>分组，添加transform="translate(l,t)"，
 *                       通过clipPath实现裁剪
 * getviewsettings()     返回当前viewport结构体信息
 * getmaxx()             返回SVG画布宽度-1
 * getmaxy()             返回SVG画布高度-1
 * getx()/gety()         返回当前画笔位置(CP)
 * moveto()              更新内部CP坐标，不绘制
 * moverel()             相对移动CP
 *
 * 三、颜色函数
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * setcolor()            设置当前stroke颜色，映射16色BGI色到#RRGGBB
 * setbkcolor()          设置背景色，绘制背景矩形
 * getcolor()            返回当前前景色索引
 * getbkcolor()          返回当前背景色索引
 * getmaxcolor()         返回最大颜色值15(16色模式)
 * setpalette()          修改调色板颜色映射表
 * setallpalette()       批量设置调色板
 *
 * 四、像素函数
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * putpixel()            <rect x="x" y="y" width="1" height="1" fill="color"/>
 * getpixel()            从内部像素缓冲区读取颜色值(简化实现返回当前色)
 *
 * 五、直线与线型函数
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * line()                <line x1 y1 x2 y2 stroke stroke-width/>
 * lineto()              <line>从当前CP到(x,y)，更新CP
 * linerel()             <line>从CP到(CP+dx,CP+dy)，更新CP
 * setlinestyle()        stroke-dasharray(虚线样式) + stroke-width(线宽)
 * getlinesettings()     返回当前线型结构体
 * setwritemode()        COPY_PUT=正常绘制/XOR_PUT=用mix-blend-mode模拟
 *
 *   线型映射(BGI -> SVG):
 *     SOLID_LINE   (0) -> stroke-dasharray="none"
 *     DOTTED_LINE  (1) -> stroke-dasharray="1,3"
 *     CENTER_LINE  (2) -> stroke-dasharray="6,2,2,2"
 *     DASHED_LINE  (3) -> stroke-dasharray="6,3"
 *     USERBIT_LINE (4) -> 根据upattern位模式生成自定义dash
 *
 *   线宽映射:
 *     NORM_WIDTH  (1) -> stroke-width="1"
 *     THICK_WIDTH (3) -> stroke-width="3"
 *
 * 六、多边形与矩形函数
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * rectangle()           <rect x y width height fill="none" stroke/>
 * bar()                 <rect ... fill="fillcolor" stroke="none"/>
 * bar3d()               <rect>+<polygon>组合绘制带45度斜角的3D条形
 * drawpoly()            <polyline points="..." fill="none" stroke/>
 * fillpoly()            <polygon points="..." fill="fillcolor" stroke/>
 *
 * 七、圆弧与曲线函数
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * circle()              <circle cx cy r stroke fill="none"/>
 * arc()                 <path d="M ... A ..." stroke fill="none"/>
 * ellipse()             <ellipse>或<path>画椭圆弧 stroke fill="none"/>
 * fillellipse()         <ellipse cx cy rx ry fill="fillcolor" stroke/>
 * pieslice()            <path d="M L A Z" 绘制填充扇形
 * sector()              <path d="M L A Z" 绘制填充椭圆扇形
 * getaspectratio()      返回xasp/yasp (SVG默认1:1即10000/10000)
 * getarccoords()        返回上一次圆弧绘制的端点坐标
 *
 * 八、填充函数
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * setfillstyle()        设置fill模式(SOLID_FILL用纯色，HATCH_FILL等用pattern)
 * setfillpattern()      用户自定义填充图案，生成<pattern>定义
 * floodfill()           泛洪填充(SVG中简化为对已有图形内部重新填充)
 * getfillsettings()     返回当前填充设置结构体
 * getfillpattern()      返回用户自定义填充图案
 *
 *   填充样式映射:
 *     EMPTY_FILL    -> fill="none"
 *     SOLID_FILL    -> fill="color" 纯色填充
 *     LINE_FILL     -> <pattern>水平线填充
 *     LTSLASH_FILL  -> <pattern>细斜线填充
 *     SLASH_FILL    -> <pattern>粗斜线填充
 *     BKSLASH_FILL  -> <pattern>粗反斜线填充
 *     LTBKSLASH_FILL-> <pattern>细反斜线填充
 *     HATCH_FILL    -> <pattern>网格线填充
 *     XHATCH_FILL   -> <pattern>交叉网格填充
 *     INTERLEAVE_FILL-><pattern>交错填充
 *     WIDE_DOT_FILL -> <pattern>疏点填充
 *     CLOSE_DOT_FILL-> <pattern>密点填充
 *     USER_FILL     -> 用户自定义pattern
 *
 * 九、文本输出函数
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * outtext()             <text x y>在当前CP位置输出，更新CP
 * outtextxy()           <text x y>在指定位置输出，不更新CP
 * settextstyle()        font-size/font-family/transform旋转(VERT_DIR)
 * settextjustify()      text-anchor/dominant-baseline控制对齐
 * textwidth()           估算字符串宽度(按字体大小*字符数*0.6)
 * textheight()          估算字符串高度(按字体大小)
 *
 *   字体映射:
 *     DEFAULT_FONT (0) -> font-family="monospace"
 *     TRIPLEX_FONT (1) -> font-family="serif"
 *     SMALL_FONT   (2) -> font-family="sans-serif"
 *     SANS_SERIF_FONT(3)->font-family="sans-serif"
 *     GOTHIC_FONT  (4) -> font-family="serif" font-weight="bold"
 *   方向:
 *     HORIZ_DIR (0) -> 正常水平
 *     VERT_DIR  (1) -> transform="rotate(-90 x y)" 垂直
 *
 * 十、图像函数(简化实现)
 * ────────────────────────────────────────────────────────────
 * graphics.h函数        SVG对应实现
 * ────────────────────────────────────────────────────────────
 * imagesize()           返回图像存储所需字节数(估算)
 * getimage()            保存区域内容(简化实现仅记录范围)
 * putimage()            <image>或引用之前保存的内容重绘
 *
 * ============================================================
 */

#ifndef SVG_GRAPHICS_H
#define SVG_GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ======================== 常量定义 ======================== */

/* 图形驱动常量 */
#define DETECT          0
#define CGA             1
#define MCGA            2
#define EGA             3
#define EGA64           4
#define EGAMONO         5
#define IBM8514         6
#define HERCMONO        7
#define ATT400          8
#define VGA             9
#define PC3270          10

/* 图形模式 */
#define CGAC0           0
#define CGAC1           1
#define CGAC2           2
#define CGAC3           3
#define CGAHI           4
#define MCGAC0          0
#define MCGAC1          1
#define MCGAC2          2
#define MCGAC3          3
#define MCGAMED         4
#define MCGAHI          5
#define EGALO           0
#define EGAHI           1
#define EGA64LO         0
#define EGA64HI         1
#define EGAMONOHI       0
#define IBM8514LO       0
#define IBM8514HI       1
#define HERCMONOHI      0
#define ATT400C0        0
#define ATT400C1        1
#define ATT400C2        2
#define ATT400C3        3
#define ATT400MED       4
#define ATT400HI        5
#define VGALO           0
#define VGAMED          1
#define VGAHI           2
#define PC3270HI        0

/* 图形错误码 */
#define grOk                0
#define grNoInitGraph      -1
#define grNotDetected      -2
#define grFileNotFound     -3
#define grInvalidDriver    -4
#define grNoLoadMem        -5
#define grNoScanMem        -6
#define grNoFloodMem       -7
#define grFontNotFound     -8
#define grNoFontMem        -9
#define grInvalidMode      -10
#define grError            -11
#define grIOerror          -12
#define grInvalidFont      -13
#define grInvalidFontNum   -14
#define grInvalidVersion   -18

/* 16色颜色常量 (EGA/VGA调色板) */
#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN     10
#define LIGHTCYAN      11
#define LIGHTRED       12
#define LIGHTMAGENTA   13
#define YELLOW         14
#define WHITE          15

/* 线型 */
#define SOLID_LINE      0
#define DOTTED_LINE     1
#define CENTER_LINE     2
#define DASHED_LINE     3
#define USERBIT_LINE    4

/* 线宽 */
#define NORM_WIDTH      1
#define THICK_WIDTH     3

/* 填充样式 */
#define EMPTY_FILL      0
#define SOLID_FILL      1
#define LINE_FILL       2
#define LTSLASH_FILL    3
#define SLASH_FILL      4
#define BKSLASH_FILL    5
#define LTBKSLASH_FILL  6
#define HATCH_FILL      7
#define XHATCH_FILL     8
#define INTERLEAVE_FILL 9
#define WIDE_DOT_FILL  10
#define CLOSE_DOT_FILL 11
#define USER_FILL      12

/* 字体 */
#define DEFAULT_FONT    0
#define TRIPLEX_FONT    1
#define SMALL_FONT      2
#define SANS_SERIF_FONT 3
#define GOTHIC_FONT     4

/* 文本方向 */
#define HORIZ_DIR       0
#define VERT_DIR        1

/* 文本对齐 */
#define LEFT_TEXT       0
#define CENTER_TEXT     1
#define RIGHT_TEXT      2
#define BOTTOM_TEXT     0
#define TOP_TEXT        2

/* 写模式 */
#define COPY_PUT        0
#define XOR_PUT         1

/* 图像操作模式 (putimage) */
#define COPY_PUT        0
#define XOR_PUT         1
#define OR_PUT          2
#define AND_PUT         3
#define NOT_PUT         4

/* ======================== 结构体定义 ======================== */

struct linesettingstype {
    int linestyle;
    unsigned upattern;
    int thickness;
};

struct fillsettingstype {
    int pattern;
    int color;
};

struct pointtype {
    int x, y;
};

struct viewporttype {
    int left, top, right, bottom;
    int clipflag;
};

struct arccoordstype {
    int x, y;
    int xstart, ystart, xend, yend;
};

struct palettetype {
    unsigned char size;
    signed char colors[16];
};

struct textsettingstype {
    int font;
    int direction;
    int charsize;
    int horiz;
    int vert;
};

/* ======================== 库内部状态结构体 ======================== */

typedef struct {
    FILE *fp;               /* SVG输出文件指针 */
    int width;              /* 画布宽度 */
    int height;             /* 画布高度 */
    int init;               /* 是否已初始化 */
    int errorcode;          /* 最后错误码 */

    /* 当前位置CP */
    int cp_x;
    int cp_y;

    /* 视口设置 */
    struct viewporttype viewport;

    /* 颜色 */
    int fg_color;           /* 当前前景色(画线/描边) */
    int bk_color;           /* 背景色 */
    const char *palette[16];/* 16色调色板映射到#RRGGBB */

    /* 线型 */
    struct linesettingstype line;

    /* 填充 */
    struct fillsettingstype fill;
    char user_pattern[8];   /* 用户自定义填充图案 */

    /* 文本 */
    struct textsettingstype text;
    int text_size;

    /* 圆弧坐标 */
    struct arccoordstype arc;

    /* 写模式 */
    int writemode;

    /* 纵横比 */
    int xasp, yasp;

    /* 图案ID计数(用于defs中唯一id) */
    int pattern_id;
} svg_graphics_state;

/* 全局状态实例 */
extern svg_graphics_state g_svg;

/* ======================== 函数声明 ======================== */

/********* 图形系统 *********/
void initgraph(int *gdriver, int *gmode, const char *pathtodriver);
void initgraph_svg(const char *filename, int width, int height);
void closegraph(void);
void detectgraph(int *gdriver, int *gmode);
int graphresult(void);
const char *grapherrormsg(int errcode);
void setgraphmode(int mode);
void restorecrtmode(void);
void cleardevice(void);
void clearviewport(void);

/********* 视口与坐标 *********/
void setviewport(int left, int top, int right, int bottom, int clip);
void getviewsettings(struct viewporttype *viewport);
int getmaxx(void);
int getmaxy(void);
int getx(void);
int gety(void);
void moveto(int x, int y);
void moverel(int dx, int dy);

/********* 颜色 *********/
void setcolor(int color);
void setbkcolor(int color);
int getcolor(void);
int getbkcolor(void);
int getmaxcolor(void);
void setpalette(int colornum, int color);
void setallpalette(struct palettetype *palette);

/********* 像素 *********/
void putpixel(int x, int y, int color);
int getpixel(int x, int y);

/********* 直线与线型 *********/
void line(int x1, int y1, int x2, int y2);
void lineto(int x, int y);
void linerel(int dx, int dy);
void setlinestyle(int linestyle, unsigned upattern, int thickness);
void getlinesettings(struct linesettingstype *lineinfo);
void setwritemode(int mode);

/********* 矩形与多边形 *********/
void rectangle(int left, int top, int right, int bottom);
void bar(int left, int top, int right, int bottom);
void bar3d(int left, int top, int right, int bottom, int depth, int topflag);
void drawpoly(int numpoints, const int *polypoints);
void fillpoly(int numpoints, const int *polypoints);

/********* 圆弧曲线 *********/
void getaspectratio(int *xasp, int *yasp);
void circle(int x, int y, int radius);
void arc(int x, int y, int stangle, int endangle, int radius);
void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius);
void fillellipse(int x, int y, int xradius, int yradius);
void pieslice(int x, int y, int stangle, int endangle, int radius);
void sector(int x, int y, int stangle, int endangle, int xradius, int yradius);
void getarccoords(struct arccoordstype *arccoords);

/********* 填充 *********/
void setfillstyle(int pattern, int color);
void setfillpattern(const char *upattern, int color);
void floodfill(int x, int y, int border);
void getfillsettings(struct fillsettingstype *fillinfo);
void getfillpattern(char *pattern);

/********* 文本 *********/
void outtext(const char *textstring);
void outtextxy(int x, int y, const char *textstring);
void settextstyle(int font, int direction, int charsize);
void settextjustify(int horiz, int vert);
int textwidth(const char *textstring);
int textheight(const char *textstring);

/********* 图像(简化) *********/
unsigned imagesize(int left, int top, int right, int bottom);
void getimage(int left, int top, int right, int bottom, void *bitmap);
void putimage(int left, int top, void *bitmap, int op);

/********* 辅助函数 *********/
const char *svg_color(int idx);
void svg_write_header(int width, int height);
void svg_write_footer(void);
void svg_write_defs_pattern(int pattern_type, int color);

/********** 随机数 *********/
int random(int n);

/********** 动画 *********/
void animate();
// 新建一帧
void framebegin();
// 结束一帧
void frameend();

#ifdef __cplusplus
}
#endif

#endif /* SVG_GRAPHICS_H */
