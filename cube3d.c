// cube3d.c
// 编译：gcc cube3d.c -o cube3d -lm
// 运行：./cube3d
// 输出：cube.ppm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define WIDTH  400
#define HEIGHT 400

static unsigned char framebuffer[HEIGHT][WIDTH][3];
static float depthbuffer[HEIGHT][WIDTH];

// ============================================================
// 1. 数学基础：向量、矩阵
// ============================================================

typedef struct { float x, y, z; } Vec3;
typedef struct { float x, y, z, w; } Vec4;

typedef struct {
    float m[4][4];
} Mat4;

// 单位矩阵
Mat4 mat4_identity(void) {
    Mat4 r;
    memset(&r, 0, sizeof(r));
    r.m[0][0] = r.m[1][1] = r.m[2][2] = r.m[3][3] = 1.0f;
    return r;
}

// 矩阵乘法：a * b
Mat4 mat4_mul(Mat4 a, Mat4 b) {
    Mat4 r;
    memset(&r, 0, sizeof(r));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                r.m[i][j] += a.m[i][k] * b.m[k][j];
    return r;
}

// 矩阵 × 向量
Vec4 mat4_mul_vec4(Mat4 m, Vec4 v) {
    Vec4 r;
    r.x = m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z + m.m[0][3]*v.w;
    r.y = m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z + m.m[1][3]*v.w;
    r.z = m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z + m.m[2][3]*v.w;
    r.w = m.m[3][0]*v.x + m.m[3][1]*v.y + m.m[3][2]*v.z + m.m[3][3]*v.w;
    return r;
}

// 平移矩阵
Mat4 mat4_translate(float tx, float ty, float tz) {
    Mat4 r = mat4_identity();
    r.m[0][3] = tx;
    r.m[1][3] = ty;
    r.m[2][3] = tz;
    return r;
}

// 绕 X 轴旋转
Mat4 mat4_rotate_x(float angle) {
    Mat4 r = mat4_identity();
    float c = cosf(angle), s = sinf(angle);
    r.m[1][1] =  c; r.m[1][2] = -s;
    r.m[2][1] =  s; r.m[2][2] =  c;
    return r;
}

// 绕 Y 轴旋转
Mat4 mat4_rotate_y(float angle) {
    Mat4 r = mat4_identity();
    float c = cosf(angle), s = sinf(angle);
    r.m[0][0] =  c; r.m[0][2] =  s;
    r.m[2][0] = -s; r.m[2][2] =  c;
    return r;
}

// 透视投影矩阵
// fov: 视野角度（弧度）
// aspect: 宽高比
// near, far: 近远裁剪面
Mat4 mat4_perspective(float fov, float aspect, float near, float far) {
    Mat4 r;
    memset(&r, 0, sizeof(r));
    float f = 1.0f / tanf(fov / 2.0f);
    r.m[0][0] = f / aspect;
    r.m[1][1] = f;
    r.m[2][2] = (far + near) / (near - far);
    r.m[2][3] = (2.0f * far * near) / (near - far);
    r.m[3][2] = -1.0f;
    return r;
}

// ============================================================
// 2. 帧缓冲操作
// ============================================================

void clear(void) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            framebuffer[y][x][0] = 20;
            framebuffer[y][x][1] = 20;
            framebuffer[y][x][2] = 30;
            depthbuffer[y][x] = 1e9f;
        }
    }
}

void set_pixel(int x, int y, float z, unsigned char r, unsigned char g, unsigned char b) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    if (z >= depthbuffer[y][x]) return;  // 深度测试
    depthbuffer[y][x] = z;
    framebuffer[y][x][0] = r;
    framebuffer[y][x][1] = g;
    framebuffer[y][x][2] = b;
}

// ============================================================
// 3. 光栅化：屏幕坐标 + 深度插值 + 颜色插值
// ============================================================

typedef struct {
    float x, y, z;        // 屏幕坐标 + 深度
    float r, g, b;        // 颜色
} ScreenVertex;

// 边函数
float edge(float ax, float ay, float bx, float by, float px, float py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

// 光栅化一个三角形
void rasterize_triangle(ScreenVertex v0, ScreenVertex v1, ScreenVertex v2) {
    // 包围盒
    int minX = (int)floorf(fminf(v0.x, fminf(v1.x, v2.x)));
    int maxX = (int)ceilf (fmaxf(v0.x, fmaxf(v1.x, v2.x)));
    int minY = (int)floorf(fminf(v0.y, fminf(v1.y, v2.y)));
    int maxY = (int)ceilf (fmaxf(v0.y, fmaxf(v1.y, v2.y)));

    if (minX < 0) minX = 0;
    if (minY < 0) minY = 0;
    if (maxX >= WIDTH)  maxX = WIDTH  - 1;
    if (maxY >= HEIGHT) maxY = HEIGHT - 1;

    // 整个三角形的面积（用于归一化）
    float area = edge(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
    if (fabsf(area) < 1e-6f) return;  // 退化三角形

    // 遍历包围盒
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            float px = x + 0.5f;
            float py = y + 0.5f;

            // 重心坐标
            float w0 = edge(v1.x, v1.y, v2.x, v2.y, px, py) / area;
            float w1 = edge(v2.x, v2.y, v0.x, v0.y, px, py) / area;
            float w2 = edge(v0.x, v0.y, v1.x, v1.y, px, py) / area;

            // 在三角形内？
            if (w0 < 0 || w1 < 0 || w2 < 0) continue;

            // 插值深度
            float z = w0 * v0.z + w1 * v1.z + w2 * v2.z;

            // 插值颜色
            float r = w0 * v0.r + w1 * v1.r + w2 * v2.r;
            float g = w0 * v0.g + w1 * v1.g + w2 * v2.g;
            float b = w0 * v0.b + w1 * v1.b + w2 * v2.b;

            set_pixel(x, y, z,
                      (unsigned char)(r * 255),
                      (unsigned char)(g * 255),
                      (unsigned char)(b * 255));
        }
    }
}

// ============================================================
// 4. 顶点变换：MVP + 透视除法 + 视口变换
// ============================================================

// 把 3D 顶点变换到屏幕空间
ScreenVertex transform_vertex(Vec3 pos, Vec3 color, Mat4 mvp) {
    // 1. 齐次坐标
    Vec4 p = {pos.x, pos.y, pos.z, 1.0f};

    // 2. MVP 变换
    Vec4 clip = mat4_mul_vec4(mvp, p);

    // 3. 透视除法 → NDC
    Vec3 ndc;
    ndc.x = clip.x / clip.w;
    ndc.y = clip.y / clip.w;
    ndc.z = clip.z / clip.w;

    // 4. 视口变换 → 屏幕坐标
    ScreenVertex sv;
    sv.x = (ndc.x + 1.0f) * 0.5f * WIDTH;
    sv.y = (1.0f - ndc.y) * 0.5f * HEIGHT;  // 屏幕 y 向下
    sv.z = ndc.z;
    sv.r = color.x;
    sv.g = color.y;
    sv.b = color.z;
    return sv;
}

// ============================================================
// 5. 立方体数据
// ============================================================

// 8 个顶点
Vec3 cube_vertices[8] = {
    {-1, -1, -1}, { 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1},  // 后面
    {-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, {-1,  1,  1},  // 前面
};

// 12 个三角形（每个面 2 个），索引到 cube_vertices
int cube_indices[12][3] = {
    // 后面
    {0, 1, 2}, {0, 2, 3},
    // 前面
    {4, 6, 5}, {4, 7, 6},
    // 左面
    {0, 3, 7}, {0, 7, 4},
    // 右面
    {1, 5, 6}, {1, 6, 2},
    // 下面
    {0, 4, 5}, {0, 5, 1},
    // 上面
    {3, 2, 6}, {3, 6, 7},
};

// 每个面的颜色
Vec3 face_colors[12] = {
    {1, 0, 0}, {1, 0, 0},  // 后：红
    {0, 1, 0}, {0, 1, 0},  // 前：绿
    {0, 0, 1}, {0, 0, 1},  // 左：蓝
    {1, 1, 0}, {1, 1, 0},  // 右：黄
    {1, 0, 1}, {1, 0, 1},  // 下：洋红
    {0, 1, 1}, {0, 1, 1},  // 上：青
};

// ============================================================
// 6. 输出 PPM
// ============================================================

void write_ppm(const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) { perror("fopen"); return; }
    fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(framebuffer, 1, sizeof(framebuffer), fp);
    fclose(fp);
    printf("已写入 %s\n", filename);
}

// ============================================================
// 7. 主程序
// ============================================================

int main(void) {
    clear();

    // ---- 构建 MVP 矩阵 ----

    // 模型矩阵：绕 Y 轴和 X 轴旋转
    float angle = 0.6f;  // 弧度
    Mat4 model = mat4_mul(mat4_rotate_y(angle), mat4_rotate_x(angle * 0.5f));

    // 视图矩阵：把相机往后移
    Mat4 view = mat4_translate(0, 0, -6);

    // 投影矩阵
    Mat4 proj = mat4_perspective(60.0f * 3.14159f / 180.0f,
                                 (float)WIDTH / HEIGHT,
                                 0.1f, 100.0f);

    // MVP = proj * view * model
    Mat4 mvp = mat4_mul(proj, mat4_mul(view, model));

    // ---- 变换所有顶点 ----

    ScreenVertex screen_verts[8];
    for (int i = 0; i < 8; i++) {
        screen_verts[i] = transform_vertex(
            cube_vertices[i],
            (Vec3){1, 1, 1},  // 占位颜色，后面按面覆盖
            mvp);
    }

    // ---- 光栅化 12 个三角形 ----

    for (int i = 0; i < 12; i++) {
        int i0 = cube_indices[i][0];
        int i1 = cube_indices[i][1];
        int i2 = cube_indices[i][2];

        ScreenVertex v0 = screen_verts[i0];
        ScreenVertex v1 = screen_verts[i1];
        ScreenVertex v2 = screen_verts[i2];

        // 按面赋颜色
        v0.r = face_colors[i].x; v0.g = face_colors[i].y; v0.b = face_colors[i].z;
        v1.r = face_colors[i].x; v1.g = face_colors[i].y; v1.b = face_colors[i].z;
        v2.r = face_colors[i].x; v2.g = face_colors[i].y; v2.b = face_colors[i].z;

        rasterize_triangle(v0, v1, v2);
    }

    write_ppm("cube.ppm");
    return 0;
}