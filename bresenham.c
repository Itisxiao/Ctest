// bresenham.c
// 编译：gcc bresenham.c -o bresenham -lm
// 运行：./bresenham
// 输出：line.ppm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH  640
#define HEIGHT 480

// 帧缓冲：每个像素 3 字节（R, G, B）
static unsigned char framebuffer[HEIGHT][WIDTH][3];

// ---------- 基础操作 ----------

// 用指定颜色清屏
void clear(unsigned char r, unsigned char g, unsigned char b) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            framebuffer[y][x][0] = r;
            framebuffer[y][x][1] = g;
            framebuffer[y][x][2] = b;
        }
    }
}

// 设置一个像素的颜色（带边界检查）
void set_pixel(int x, int y,
               unsigned char r, unsigned char g, unsigned char b) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    framebuffer[y][x][0] = r;
    framebuffer[y][x][1] = g;
    framebuffer[y][x][2] = b;
}

// ---------- Bresenham 画线 ----------

// 通用版本：支持任意方向的直线
void draw_line(int x0, int y0, int x1, int y1,
               unsigned char r, unsigned char g, unsigned char b) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;   // x 方向步进
    int sy = (y0 < y1) ? 1 : -1;   // y 方向步进
    int err = dx - dy;             // 误差项

    while (1) {
        set_pixel(x0, y0, r, g, b);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// ---------- 输出 PPM ----------

// PPM P6 格式：二进制 RGB
// 文件头：
//   P6\n
//   <width> <height>\n
//   255\n
// 然后是 width*height*3 字节的 RGB 数据
void write_ppm(const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return;
    }

    // 文件头
    fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);

    // 像素数据
    fwrite(framebuffer, 1, sizeof(framebuffer), fp);

    fclose(fp);
    printf("已写入 %s (%d x %d)\n", filename, WIDTH, HEIGHT);
}

// ---------- 主程序 ----------

int main(void) {
    // 1. 清屏为深灰色
    clear(30, 30, 30);

    // 2. 画几条不同方向的线，不同颜色

    // 水平线（红色）
    draw_line(50, 50, 590, 50, 255, 0, 0);

    // 垂直线（绿色）
    draw_line(50, 50, 50, 430, 0, 255, 0);

    // 45 度斜线（蓝色）
    draw_line(50, 50, 430, 430, 0, 128, 255);

    // 缓坡斜线（黄色）
    draw_line(50, 200, 590, 250, 255, 255, 0);

    // 陡坡斜线（洋红）
    draw_line(200, 50, 250, 430, 255, 0, 255);

    // 反向斜线（青色）
    draw_line(590, 50, 50, 430, 0, 255, 255);

    // 3. 画一个"星形"：从中心到四周
    int cx = WIDTH / 2;
    int cy = HEIGHT / 2;
    for (int i = 0; i < 16; i++) {
        double angle = i * (2.0 * M_PI / 16);
        int x = cx + (int)(200 * cos(angle));
        int y = cy + (int)(200 * sin(angle));
        draw_line(cx, cy, x, y, 255, 255, 255);
    }

    // 4. 输出 PPM
    write_ppm("line.ppm");

    return 0;
}