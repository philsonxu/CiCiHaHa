/**
 * @file common.c
 * @brief 公共工具模块实现
 */
#include "common.h"

/* ---------------- 内存分配 ---------------- */
Matrix* mat_create(int rows, int cols) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        m->data[i] = (double*)calloc(cols, sizeof(double));
    }
    return m;
}

void mat_free(Matrix *m) {
    if (!m) return;
    for (int i = 0; i < m->rows; i++) free(m->data[i]);
    free(m->data);
    free(m);
}

Vector* vec_create(int size) {
    Vector *v = (Vector*)malloc(sizeof(Vector));
    v->size = size;
    v->data = (double*)calloc(size, sizeof(double));
    return v;
}

void vec_free(Vector *v) {
    if (!v) return;
    free(v->data);
    free(v);
}

/* ---------------- 取值赋值 ---------------- */
void mat_set(Matrix *m, int i, int j, double val) { m->data[i][j] = val; }
double mat_get(const Matrix *m, int i, int j) { return m->data[i][j]; }
void vec_set(Vector *v, int i, double val) { v->data[i] = val; }
double vec_get(const Vector *v, int i) { return v->data[i]; }

Matrix* mat_copy(const Matrix *src) {
    Matrix *dst = mat_create(src->rows, src->cols);
    for (int i = 0; i < src->rows; i++)
        for (int j = 0; j < src->cols; j++)
            dst->data[i][j] = src->data[i][j];
    return dst;
}

Vector* vec_copy(const Vector *src) {
    Vector *dst = vec_create(src->size);
    for (int i = 0; i < src->size; i++) dst->data[i] = src->data[i];
    return dst;
}

/* ---------------- 打印 ---------------- */
void mat_print(const char *name, const Matrix *m, int precision) {
    printf("\n===== %s (%d x %d) =====\n", name, m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        printf("  [");
        for (int j = 0; j < m->cols; j++) {
            printf("%*.*f", precision + 6, precision, m->data[i][j]);
        }
        printf(" ]\n");
    }
}

void vec_print(const char *name, const Vector *v, int precision) {
    printf("\n===== %s (size=%d) =====\n", name, v->size);
    printf("  [");
    for (int i = 0; i < v->size; i++) printf("%*.*f", precision + 4, precision, v->data[i]);
    printf(" ]\n");
}

void print_result(const char *title, double computed, double expected, double eps) {
    double err = fabs(computed - expected);
    printf("%-35s | 计算值: %12.8f | 理论值: %12.8f | 误差: %12.2e | %s\n",
           title, computed, expected, err,
           err < eps ? "✓ PASS" : "✗ FAIL");
}

/* ---------------- 误差与验证 ---------------- */
double vec_norm2(const Vector *v) {
    double s = 0.0;
    for (int i = 0; i < v->size; i++) s += v->data[i] * v->data[i];
    return sqrt(s);
}

double vec_error_l2(const Vector *computed, const Vector *expected) {
    if (computed->size != expected->size) return -1.0;
    double s = 0.0;
    for (int i = 0; i < computed->size; i++) {
        double d = computed->data[i] - expected->data[i];
        s += d * d;
    }
    return sqrt(s);
}

double mat_error_fro(const Matrix *computed, const Matrix *expected) {
    if (computed->rows != expected->rows || computed->cols != expected->cols) return -1.0;
    double s = 0.0;
    for (int i = 0; i < computed->rows; i++)
        for (int j = 0; j < computed->cols; j++) {
            double d = computed->data[i][j] - expected->data[i][j];
            s += d * d;
        }
    return sqrt(s);
}

int verify_double(double computed, double expected, double eps) {
    return fabs(computed - expected) < eps;
}

int verify_vector(const Vector *computed, const Vector *expected, double eps) {
    return vec_error_l2(computed, expected) < eps;
}

int verify_matrix(const Matrix *computed, const Matrix *expected, double eps) {
    return mat_error_fro(computed, expected) < eps;
}

/* ---------------- 文件IO ---------------- */
Matrix* mat_read(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror(filename); return NULL; }
    int rows, cols;
    fscanf(fp, "%d %d", &rows, &cols);
    Matrix *m = mat_create(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            fscanf(fp, "%lf", &m->data[i][j]);
    fclose(fp);
    return m;
}

Vector* vec_read(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror(filename); return NULL; }
    int size;
    fscanf(fp, "%d", &size);
    Vector *v = vec_create(size);
    for (int i = 0; i < size; i++) fscanf(fp, "%lf", &v->data[i]);
    fclose(fp);
    return v;
}

int mat_write(const char *filename, const Matrix *m) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return -1;
    fprintf(fp, "%d %d\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) fprintf(fp, "%.15e ", m->data[i][j]);
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

int vec_write(const char *filename, const Vector *v) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return -1;
    fprintf(fp, "%d\n", v->size);
    for (int i = 0; i < v->size; i++) fprintf(fp, "%.15e\n", v->data[i]);
    fclose(fp);
    return 0;
}

/* ---------------- 计时 ---------------- */
double get_time_sec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}
