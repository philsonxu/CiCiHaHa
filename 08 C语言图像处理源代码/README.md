# C语言图像处理源代码库 (imageproc)

一套**零第三方依赖**、纯C11实现的轻量级图像处理库，从底层BMP/PPM/PGM文件读写到高级算法（Canny边缘、Otsu二值化、形态学、高斯/中值滤波、双线性插值、直方图均衡化、PSNR定量评估）全部实现，并配有 **8个示例程序 + 3个验证测试程序**。

## 目录结构

```
C语言图像处理源代码/
├── include/imageproc.h        # 核心头文件（所有API声明）
├── src/                       # 核心库源码（编译为libimageproc.a静态库）
│   ├── core.c                 # 图像创建/释放/克隆、测试图案
│   ├── bmp_io.c               # BMP 24位读写（4字节行对齐、top-down/bottom-up）
│   ├── ppm_pgm_io.c           # Netpbm PPM(P3/P6)/PGM(P2/P5)读写
│   ├── adjust.c               # 亮度/对比度/反色、RGB↔Gray转换
│   ├── geometry.c             # 翻转/旋转/最近邻+双线性缩放/裁剪
│   ├── histogram.c            # 直方图统计/均衡化/ASCII可视化
│   ├── filter.c               # 3×3卷积、方框/高斯/中值滤波、Sobel、Canny
│   ├── morphology.c           # Otsu阈值、腐蚀/膨胀/开/闭运算
│   ├── draw.c                 # Bresenham直线、中点圆、矩形、像素点
│   └── noise_stat.c           # 高斯/椒盐噪声、MSE/PSNR、均值/标准差
├── examples/                  # 8个应用实例程序
│   ├── demo_basic.c           # 基础IO与像素调整
│   ├── demo_filters.c         # 各种滤波效果（含自定义浮雕卷积核）
│   ├── demo_edges.c           # Sobel + Canny边缘检测
│   ├── demo_morphology.c      # Otsu二值化 + 形态学操作
│   ├── demo_noise_denoise.c   # 噪声添加与去噪PSNR定量对比
│   ├── demo_geometry.c        # 翻转/旋转/缩放/裁剪
│   ├── demo_paint.c           # 绘图API实战（靶心/棋盘格/时钟/随机点）
│   └── demo_histeq.c          # 直方图均衡化前后对比
├── tests/                     # 3个验证测试程序（带assert单元测试）
│   ├── test_io.c              # IO round-trip、对齐、错误处理
│   ├── test_filters.c         # 卷积数学正确性、几何性质、PSNR指标
│   └── test_pipeline.c        # 端到端综合流水线验证
├── test_images/               # 测试图片资源（可自行放入BMP/PPM测试）
└── Makefile
```

## 编译与运行

```bash
make              # 编译静态库 + 所有示例 + 所有测试
make run          # 运行全部8个示例，结果输出到 output/ 目录
make run-tests    # 运行全部3个验证程序（有assert断言，通过会打印PASS）
make clean        # 清理编译产物
```

## 支持功能一览

| 类别 | 函数 | 说明 |
|------|------|------|
| **IO** | `ip_bmp_load/save`, `ip_ppm_load/save`, `ip_pgm_load/save` | BMP24 / PPM(P3/P6) / PGM(P2/P5) |
| **内存管理** | `ip_image_create/release/clone` | 引用安全，自动分配/释放 |
| **像素访问** | `IP_R/G/B/GRAY/PIXEL` 宏 | 安全快速访问像素 |
| **格式转换** | `ip_rgb_to_gray`, `ip_gray_to_rgb`, 平均法/NTSC加权 | 单/三通道互转 |
| **几何变换** | 水平/垂直翻转、90°/180°旋转、最近邻/双线性缩放、裁剪 | 双线性插值平滑 |
| **像素调整** | 亮度、对比度、反色 | 带clamp防溢出 |
| **直方图** | 统计、均衡化、ASCII可视化 | CDF标准均衡化 |
| **滤波** | 3×3通用卷积、方框、高斯、中值、拉普拉斯锐化、Sobel、Canny | Canny完整四步实现 |
| **二值化/形态学** | 固定阈值、Otsu自动阈值、腐蚀/膨胀/开/闭 | k×k矩形结构元 |
| **绘图** | 点、Bresenham直线、矩形、中点圆(填充) | 8对称圆算法 |
| **噪声/统计** | 高斯噪声(Box-Muller)、椒盐噪声、MSE/PSNR、均值/标准差 | 定量评价 |

## 快速上手示例

```c
#include "imageproc.h"
#include <stdio.h>

int main() {
    /* 1. 生成测试图案 */
    Image *img = NULL;
    ip_create_test_pattern(400, 300, &img);

    /* 2. 保存为BMP */
    ip_bmp_save("hello.bmp", img);

    /* 3. 高斯模糊 */
    Image *blur = NULL;
    ip_blur_gaussian(img, 5, 1.4, &blur);
    ip_bmp_save("hello_blur.bmp", blur);

    /* 4. Canny边缘 */
    Image *gray = NULL, *edges = NULL;
    ip_rgb_to_gray(blur, &gray);
    ip_edge_canny(gray, 50, 100, &edges);
    Image *edges_rgb = NULL;
    ip_gray_to_rgb(edges, &edges_rgb);
    ip_bmp_save("hello_edges.bmp", edges_rgb);

    /* 5. 计算PSNR */
    double psnr;
    ip_mse_psnr(img, blur, NULL, &psnr);
    printf("模糊后PSNR=%.2f dB\n", psnr);

    /* 释放 */
    ip_image_release(&img); ip_image_release(&blur);
    ip_image_release(&gray); ip_image_release(&edges); ip_image_release(&edges_rgb);
    return 0;
}
```

编译：`gcc demo.c -Iinclude -L. -limageproc -lm -o demo`

## 验证覆盖

- ✅ **BMP round-trip无损**：11种宽度覆盖所有4字节对齐余数，MSE=0
- ✅ **PPM P6/P3 + PGM P5/P2**：二进制与文本格式双向无损
- ✅ **卷积数学性质**：恒等核输出=输入、高斯核和=1、均值核均匀区域无影响
- ✅ **几何性质**：水平+垂直翻转 ≡ 180°旋转（MSE=0）、反色两次还原
- ✅ **滤波效果**：中值滤波对10%椒盐噪声PSNR提升>10dB
- ✅ **Otsu二值化**：输出严格只有0/255
- ✅ **完整流水线**：带噪声图→去噪→灰度→均衡→二值→形态学→边缘，每步尺寸/通道/数据范围均验证

## 设计说明

1. **零依赖**：仅用C标准库 + `libm`，Linux/macOS/MinGW可直接编译运行
2. **BMP 24位**：兼容所有图片查看器（Windows照片、macOS预览、GIMP等直接打开）
3. **内部BGR序**：与BMP字节序一致，省去读写时的通道交换；用`IP_R/IP_G/IP_B`宏访问
4. **边界处理**：所有卷积/滤波均使用"复制边界"填充（replicate padding），不会出现黑边
5. **中文注释详尽**：核心算法（Canny四步、Otsu类间方差、Bresenham、中点圆、Box-Muller）均有详细注释
6. **错误处理**：空指针/越界/格式不匹配等均返回状态码，不直接crash
