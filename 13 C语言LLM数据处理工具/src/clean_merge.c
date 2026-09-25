/*
 * clean_merge.c
 * C语言实现：LLM训练数据清洗 + 训练集/验证集划分
 *
 * 功能：
 *   1. 递归扫描 data/raw/zh 和 data/raw/en 下的 .txt 文件
 *   2. 逐行清洗：去除空行、连续空白、控制字符
 *   3. 过滤 Project Gutenberg 版权首尾声明
 *   4. 文档间用 "<|endoftext|>" 分隔（GPT标准格式）
 *   5. 按 --val_ratio (默认0.05) 切分为 train.txt / eval.txt
 *
 * 编译：gcc -O2 -o clean_merge clean_merge.c -std=c11 -Wall
 * 用法：
 *   ./clean_merge                       # 使用默认参数
 *   ./clean_merge --raw_dir data/raw --out_dir data/cleaned --val_ratio 0.05
 *   ./clean_merge --seed 42
 */

#define _DEFAULT_SOURCE   /* opendir/stat/strdup */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

/* ===== 可调参数（命令行可覆盖） ===== */
static const char *g_raw_dir     = "data/raw";
static const char *g_out_dir     = "data";            /* train.txt/eval.txt 直接写到 data/ */
static double      g_val_ratio   = 0.05;
static unsigned int g_seed       = 0;                 /* 0 = 用时间戳 */

/* ===== 全局统计 ===== */
static long g_total_files   = 0;
static long g_total_chars   = 0;
static long g_total_lines   = 0;
static long g_kept_chars    = 0;
static long g_train_chars   = 0;
static long g_eval_chars    = 0;

/* ===== 动态字符串 buffer ===== */
typedef struct {
    char  *buf;
    size_t len;
    size_t cap;
} DStr;

static void dstr_init(DStr *s) {
    s->cap = 1 << 16; /* 64KB */
    s->buf = (char*)malloc(s->cap);
    s->len = 0;
    s->buf[0] = '\0';
}
static void dstr_free(DStr *s) {
    free(s->buf);
    s->buf = NULL; s->len = s->cap = 0;
}
static void dstr_append(DStr *s, const char *add, size_t n) {
    if (s->len + n + 1 > s->cap) {
        while (s->len + n + 1 > s->cap) s->cap *= 2;
        s->buf = (char*)realloc(s->buf, s->cap);
    }
    memcpy(s->buf + s->len, add, n);
    s->len += n;
    s->buf[s->len] = '\0';
}
static void dstr_append_cstr(DStr *s, const char *add) {
    dstr_append(s, add, strlen(add));
}

/* ===== 去除字符串首尾空白（原地） ===== */
static char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    if (!*s) return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) *end-- = '\0';
    return s;
}

/* ===== 判断一行是否为 Gutenberg 版权标记边界 ===== */
static int is_gutenberg_marker(const char *line) {
    /* 匹配 *** START OF ... *** / *** END OF ... *** */
    if (strstr(line, "*** START OF") && strstr(line, "***")) return 1;
    if (strstr(line, "*** END OF")   && strstr(line, "***")) return 1;
    return 0;
}

/* ===== 清洗单个文件内容，追加到 DStr ===== */
static int clean_one_file(const char *filepath, DStr *out) {
    FILE *fp = fopen(filepath, "rb");
    if (!fp) { fprintf(stderr, "  [WARN] 无法打开: %s\n", filepath); return -1; }

    /* 逐行读取 */
    char line[8192];
    int  in_gutenberg_preamble = 1; /* 默认先跳过前版权 */
    int  skipped_start = 0;
    long kept = 0;

    while (fgets(line, sizeof(line), fp)) {
        g_total_lines++;
        char *t = trim(line);
        size_t L = strlen(t);
        if (L == 0) continue; /* 空行跳过 */

        /* Gutenberg 边界处理 */
        if (is_gutenberg_marker(t)) {
            if (!skipped_start) { in_gutenberg_preamble = 0; skipped_start = 1; continue; }
            else { break; /* 遇到 END OF 停止读 */ }
        }
        if (in_gutenberg_preamble) continue;

        /* 把连续空白压成单空格（保留字符原样，但压缩空白） */
        char cleaned[8192];
        size_t ci = 0;
        int prev_space = 0;
        for (size_t i = 0; i < L && ci < sizeof(cleaned) - 2; i++) {
            unsigned char c = (unsigned char)t[i];
            if (c < 0x20 || c == 0x7f) continue; /* 控制字符去除（保留 \t 的话这里改） */
            if (c == 0x0d) continue;              /* 回车 */
            if (isspace(c)) {
                if (!prev_space) { cleaned[ci++] = ' '; prev_space = 1; }
            } else {
                cleaned[ci++] = c; prev_space = 0;
            }
        }
        /* 末尾去掉尾部空格 */
        while (ci > 0 && cleaned[ci-1] == ' ') ci--;
        if (ci == 0) continue;
        cleaned[ci] = '\0';

        dstr_append(out, cleaned, ci);
        dstr_append_cstr(out, "\n");
        kept += ci + 1;
        g_total_chars += L;
    }
    /* 文档结束分隔符 */
    dstr_append_cstr(out, "<|endoftext|>\n");
    kept += 14;

    fclose(fp);
    g_kept_chars += kept;
    g_total_files++;
    return 0;
}

/* ===== 递归收集目录下所有 .txt 文件 ===== */
static void collect_txt(const char *dirpath, DStr *out) {
    DIR *d = opendir(dirpath);
    if (!d) { fprintf(stderr, "  [WARN] 无法打开目录: %s\n", dirpath); return; }
    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;
        char path[4096];
        snprintf(path, sizeof(path), "%s/%s", dirpath, e->d_name);
        struct stat st;
        if (stat(path, &st) != 0) continue;
        if (S_ISDIR(st.st_mode)) {
            collect_txt(path, out);
        } else if (S_ISREG(st.st_mode)) {
            size_t nl = strlen(e->d_name);
            if (nl > 4 && strcmp(e->d_name + nl - 4, ".txt") == 0) {
                clean_one_file(path, out);
            }
        }
    }
    closedir(d);
}

/* ===== Fisher-Yates 文档级打乱：按 <|endoftext|>\n 分片后随机重排 ===== */
static void shuffle_docs(char *buf, size_t len, size_t *n_docs_out) {
    size_t cap = 1024;
    char **d = (char**)malloc(cap * sizeof(char*));
    size_t *dl = (size_t*)malloc(cap * sizeof(size_t));
    size_t n = 0;
    const char *sep = "<|endoftext|>\n";
    size_t sepl = strlen(sep);
    char *p = buf;
    char *start = buf;
    while (p + sepl <= buf + len) {
        if (memcmp(p, sep, sepl) == 0) {
            size_t block_len = (p + sepl) - start;
            if (n >= cap) { cap *= 2; d = (char**)realloc(d, cap*sizeof(char*)); dl = (size_t*)realloc(dl, cap*sizeof(size_t)); }
            d[n] = start; dl[n] = block_len; n++;
            p += sepl;
            start = p;
        } else {
            p++;
        }
    }
    /* 末尾剩余 */
    if (start < buf + len) {
        if (n >= cap) { cap *= 2; d = (char**)realloc(d, cap*sizeof(char*)); dl = (size_t*)realloc(dl, cap*sizeof(size_t)); }
        d[n] = start; dl[n] = buf + len - start; n++;
    }
    /* Fisher-Yates */
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        char *tp = d[i]; d[i] = d[j]; d[j] = tp;
        size_t tl = dl[i]; dl[i] = dl[j]; dl[j] = tl;
    }
    /* 原地按洗牌顺序重写到 buf 开头 */
    char *w = buf;
    for (size_t i = 0; i < n; i++) {
        memmove(w, d[i], dl[i]);
        w += dl[i];
    }
    *n_docs_out = n;
    free(d); free(dl);
}

/* ===== 命令行参数解析 ===== */
static void parse_args(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--raw_dir") == 0 && i+1 < argc)     g_raw_dir   = argv[++i];
        else if (strcmp(argv[i], "--out_dir") == 0 && i+1 < argc) g_out_dir   = argv[++i];
        else if (strcmp(argv[i], "--val_ratio") == 0 && i+1 < argc) g_val_ratio = atof(argv[++i]);
        else if (strcmp(argv[i], "--seed") == 0 && i+1 < argc)     g_seed      = (unsigned int)atoi(argv[++i]);
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("用法: %s [--raw_dir DIR] [--out_dir DIR] [--val_ratio FLOAT] [--seed N]\n", argv[0]);
            exit(0);
        }
    }
}

/* ===== 主流程 ===== */
int main(int argc, char **argv) {
    parse_args(argc, argv);
    srand(g_seed ? g_seed : (unsigned int)time(NULL));

    printf("========================================\n");
    printf("  C语言 LLM 训练数据清洗与划分\n");
    printf("========================================\n");
    printf("  原始目录 : %s\n", g_raw_dir);
    printf("  输出目录 : %s\n", g_out_dir);
    printf("  验证集比例: %.2f%%\n", g_val_ratio * 100);
    printf("  随机种子 : %u\n\n", g_seed ? g_seed : (unsigned int)time(NULL));

    DStr merged; dstr_init(&merged);

    printf("[1/3] 扫描并清洗原始 .txt 文件...\n");
    collect_txt(g_raw_dir, &merged);
    printf("  处理文件数 : %ld\n", g_total_files);
    printf("  原始字符数 : %ld\n", g_total_chars);
    printf("  清洗后字符 : %ld\n\n", g_kept_chars);

    if (g_total_files == 0) {
        fprintf(stderr, "[ERROR] 没找到任何 .txt 文件，请检查 --raw_dir\n");
        dstr_free(&merged); return 1;
    }

    printf("[2/3] 文档级随机打乱...\n");
    /* 按 <|endoftext|>\n 分片 Fisher-Yates 打乱后原地重写，避免同一篇长文
       全部落入 train 或 eval，保证验证集多样性。 */
    size_t n_docs = 0;
    shuffle_docs(merged.buf, merged.len, &n_docs);
    printf("  文档总数 : %zu\n\n", n_docs);

    printf("[3/3] 划分 train/eval 并写出...\n");
    /* 创建输出目录 */
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s/pretrain %s/eval", g_out_dir, g_out_dir);
    (void)system(cmd);

    size_t split_pos = (size_t)(merged.len * (1.0 - g_val_ratio));
    /* 在 <|endoftext|>\n 边界附近对齐，避免切在文档中间 */
    {
        const char *sep = "<|endoftext|>\n"; size_t sepl = strlen(sep);
        /* 向后找最近的分隔符结束位置 */
        for (size_t p = split_pos; p + sepl <= merged.len; p++) {
            if (memcmp(merged.buf + p, sep, sepl) == 0) { split_pos = p + sepl; break; }
        }
    }

    char train_path[4096], eval_path[4096];
    snprintf(train_path, sizeof(train_path), "%s/pretrain/train.txt", g_out_dir);
    snprintf(eval_path,  sizeof(eval_path),  "%s/eval/eval.txt",    g_out_dir);

    FILE *ft = fopen(train_path, "wb");
    FILE *fe = fopen(eval_path,  "wb");
    if (!ft || !fe) { perror("fopen"); return 1; }
    fwrite(merged.buf, 1, split_pos, ft);          g_train_chars = (long)split_pos;
    fwrite(merged.buf + split_pos, 1, merged.len - split_pos, fe); g_eval_chars  = (long)(merged.len - split_pos);
    fclose(ft); fclose(fe);

    printf("  train.txt -> %-40s %12ld 字节\n", train_path, g_train_chars);
    printf("  eval.txt  -> %-40s %12ld 字节\n", eval_path,  g_eval_chars);
    printf("\n✅ 完成！共 %.2f MB 文本。\n", (g_train_chars + g_eval_chars) / 1048576.0);

    dstr_free(&merged);
    return 0;
}
