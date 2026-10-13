// triangle_raster.c
// 编译：gcc triangle_raster.c -o triangle_raster -lm
// 运行：./triangle_raster
// 输出：triangle.ppm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH  32
#define HEIGHT 32

static unsigned char framebuffer[HEIGHT][WIDTH][3];

// ---------- 基础操作 ----------

void clear(unsigned char r, unsigned char g, unsigned char b) {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++) {
            framebuffer[y][x][0] = r;
            framebuffer[y][x][1] = g;
            framebuffer[y][x][2] = b;
        }
}

void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    framebuffer[y][x][0] = r;
    framebuffer[y][x][1] = g;
    framebuffer[y][x][2] = b;
}

// ---------- 光栅化核心 ----------

// 边函数：判断点 P 在边 AB 的哪一侧
// 返回值 > 0：P 在 AB 左侧
// 返回值 = 0：P 在 AB 上
// 返回值 < 0：P 在 AB 右侧
int edge(int ax, int ay, int bx, int by, int px, int py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

// 光栅化一个三角形
void rasterize_triangle(
    int x0, int y0,   // 顶点 0
    int x1, int y1,   // 顶点 1
    int x2, int y2,   // 顶点 2
    unsigned char r, unsigned char g, unsigned char b)
{
    // 1. 算包围盒
    int minX = x0 < x1 ? (x0 < x2 ? x0 : x2) : (x1 < x2 ? x1 : x2);
    int maxX = x0 > x1 ? (x0 > x2 ? x0 : x2) : (x1 > x2 ? x1 : x2);
    int minY = y0 < y1 ? (y0 < y2 ? y0 : y2) : (y1 < y2 ? y1 : y2);
    int maxY = y0 > y1 ? (y0 > y2 ? y0 : y2) : (y1 > y2 ? y1 : y2);

    // 2. 遍历包围盒里的每个像素
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            // 3. 用边函数判断像素中心 (x+0.5, y+0.5) 在不在三角形内
            int w0 = edge(x1, y1, x2, y2, x, y);
            int w1 = edge(x2, y2, x0, y0, x, y);
            int w2 = edge(x0, y0, x1, y1, x, y);

            // 4. 如果三个边函数都 >= 0，说明在三角形内
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                set_pixel(x, y, r, g, b);
            }
        }
    }
}

// ---------- 输出 ----------

void write_ppm(const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) { perror("fopen"); return; }
    fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(framebuffer, 1, sizeof(framebuffer), fp);
    fclose(fp);
    printf("已写入 %s (%d x %d)\n", filename, WIDTH, HEIGHT);
}

// ---------- 主程序 ----------

int main(void) {
    // 清屏为深灰
    clear(30, 30, 30);

    // 光栅化一个三角形
    // 顶点：上、左下、右下
    rasterize_triangle(
        16,  2,    // 顶点 0：上方
         2, 29,    // 顶点 1：左下
        29, 29,    // 顶点 2：右下
        255, 128, 0  // 橙色
    );

    write_ppm("triangle.ppm");
    return 0;
}