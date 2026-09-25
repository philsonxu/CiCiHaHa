/*
 * build_tokenizer.c
 * C语言实现：字符级 Tokenizer 构建 + 文本 Tokenize 二进制化
 *
 * 功能：
 *   1. 扫描 train.txt + eval.txt，自动构建字符级词表（UTF-8 按字符统计频次）
 *   2. 在词表头部预留 5 个特殊 token：<pad> <unk> <bos> <eos> <|endoftext|>
 *   3. 将词表保存为 tokenizer.json（JSON 格式，含 itos / stoi / vocab_size）
 *   4. 把 train.txt / eval.txt 编码为 uint16 二进制文件 train.bin / eval.bin
 *      （小端序，C 直接 fread 读取；Python 可用 numpy.fromfile 读取）
 *
 * UTF-8 解析：根据首字节高位 1 的个数识别 1/2/3/4 字节字符，不依赖外部库。
 *
 * 编译：gcc -O2 -o build_tokenizer build_tokenizer.c -std=c11 -Wall
 * 用法：
 *   ./build_tokenizer                               # 默认参数
 *   ./build_tokenizer --text_dir data --out_dir data/tokenized \
 *                     --max_vocab 8000 --tokenizer_path tokenizer/char_tokenizer.json
 */

#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <strings.h>

/* ===== 默认参数 ===== */
static const char *g_text_dir       = "data";
static const char *g_out_dir        = "data/tokenized";
static const char *g_tokenizer_path = "tokenizer/char_tokenizer.json";
static int         g_max_vocab      = 8000;

/* ===== 特殊 token ===== */
static const char *SPECIAL_TOKENS[] = {
    "<pad>", "<unk>", "<bos>", "<eos>", "<|endoftext|>"
};
#define N_SPECIAL (int)(sizeof(SPECIAL_TOKENS)/sizeof(SPECIAL_TOKENS[0]))
/* 特殊 token id */
#define PAD_ID 0
#define UNK_ID 1
#define BOS_ID 2
#define EOS_ID 3
#define EOT_ID 4  /* <|endoftext|> */

/* ===== UTF-8 解码：从 *p 读取一个字符，返回字符字节数，把字符写入 out（最多 4 字节+\0） ===== */
static int utf8_decode(const unsigned char *p, char *out) {
    unsigned char c = p[0];
    if (c < 0x80) { out[0] = c; out[1] = 0; return 1; }
    int nbytes;
    if      ((c & 0xE0) == 0xC0) nbytes = 2;
    else if ((c & 0xF0) == 0xE0) nbytes = 3;
    else if ((c & 0xF8) == 0xF0) nbytes = 4;
    else { out[0] = '?'; out[1] = 0; return 1; } /* 非法首字节 */
    /* 合法性校验：后续字节必须是 10xxxxxx */
    for (int i = 1; i < nbytes; i++) {
        if ((p[i] & 0xC0) != 0x80) { out[0] = '?'; out[1] = 0; return 1; }
    }
    memcpy(out, p, nbytes); out[nbytes] = 0;
    return nbytes;
}

/* ===== 简易字典：char* -> int，用于 stoi 查找（线性探查哈希表） ===== */
typedef struct {
    char    **keys;
    int      *vals;
    size_t    cap;
    size_t    size;
} Dict;

static uint64_t hash_str(const char *s) {
    /* FNV-1a 64bit */
    uint64_t h = 1469598103934665603ULL;
    while (*s) { h ^= (unsigned char)*s++; h *= 1099511628211ULL; }
    return h;
}
static void dict_init(Dict *d, size_t cap) {
    d->cap  = cap;
    d->size = 0;
    d->keys = (char**)calloc(cap, sizeof(char*));
    d->vals = (int*)malloc(cap * sizeof(int));
}
static int dict_get(Dict *d, const char *key) {
    size_t i = hash_str(key) % d->cap;
    while (d->keys[i]) {
        if (strcmp(d->keys[i], key) == 0) return d->vals[i];
        i = (i + 1) % d->cap;
    }
    return -1;
}
static void dict_put(Dict *d, const char *key, int val) {
    if ((double)d->size / d->cap > 0.7) { /* 需要扩容 */
        size_t ncap = d->cap * 2;
        char **nkeys = (char**)calloc(ncap, sizeof(char*));
        int   *nvals = (int*)malloc(ncap * sizeof(int));
        for (size_t k = 0; k < d->cap; k++) {
            if (d->keys[k]) {
                size_t i = hash_str(d->keys[k]) % ncap;
                while (nkeys[i]) i = (i+1) % ncap;
                nkeys[i] = d->keys[k]; nvals[i] = d->vals[k];
            }
        }
        free(d->keys); free(d->vals);
        d->keys = nkeys; d->vals = nvals; d->cap = ncap;
    }
    size_t i = hash_str(key) % d->cap;
    while (d->keys[i] && strcmp(d->keys[i], key) != 0) i = (i+1) % d->cap;
    if (!d->keys[i]) { d->keys[i] = strdup(key); d->size++; }
    d->vals[i] = val;
}
static void dict_free(Dict *d) {
    for (size_t i = 0; i < d->cap; i++) if (d->keys[i]) free(d->keys[i]);
    free(d->keys); free(d->vals);
    d->keys = NULL; d->vals = NULL; d->cap = d->size = 0;
}

/* ===== 频次表项 ===== */
typedef struct { char ch[8]; long cnt; } CharCnt;

static int cmp_cnt_desc(const void *a, const void *b) {
    long d = ((const CharCnt*)b)->cnt - ((const CharCnt*)a)->cnt;
    return d < 0 ? -1 : (d > 0 ? 1 : 0);
}

/* ===== 统计文件中每个 UTF-8 字符的频次 ===== */
static long count_chars(const char *path, Dict *cnt_dict) {
    FILE *fp = fopen(path, "rb");
    if (!fp) { perror(path); return 0; }
    fseek(fp, 0, SEEK_END); long sz = ftell(fp); fseek(fp, 0, SEEK_SET);
    unsigned char *buf = (unsigned char*)malloc(sz + 1);
    if (!buf) { fclose(fp); return 0; }
    size_t rd = fread(buf, 1, sz, fp); (void)rd; buf[sz] = 0;
    fclose(fp);

    long total = 0;
    size_t i = 0;
    char ch[8];
    while (i < (size_t)sz) {
        int nb = utf8_decode(buf + i, ch);
        /* 不要把 <|endoftext|> 拆开按 < | ... 统计，这里训练文本里的 <|endoftext|>
           是作为分隔符由 clean_merge 写入的，我们在 tokenize 阶段会特殊识别它并替换为 EOT_ID。
           统计频次时跳过特殊 token 字符 '<','|','>' 可以避免污染词表，但为了简单起见，
           这里直接按字符统计，tokenize 阶段再做特殊匹配即可。 */
        int old = dict_get(cnt_dict, ch);
        if (old < 0) dict_put(cnt_dict, ch, 1);
        else         dict_put(cnt_dict, ch, old + 1);
        i += nb;
        total++;
    }
    free(buf);
    return total;
}

/* ===== 命令行 ===== */
static void parse_args(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if      (strcmp(argv[i], "--text_dir") == 0 && i+1 < argc)       g_text_dir       = argv[++i];
        else if (strcmp(argv[i], "--out_dir") == 0 && i+1 < argc)        g_out_dir        = argv[++i];
        else if (strcmp(argv[i], "--tokenizer_path") == 0 && i+1 < argc) g_tokenizer_path = argv[++i];
        else if (strcmp(argv[i], "--max_vocab") == 0 && i+1 < argc)      g_max_vocab      = atoi(argv[++i]);
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("用法: %s [--text_dir DIR] [--out_dir DIR] [--tokenizer_path FILE] [--max_vocab N]\n", argv[0]);
            exit(0);
        }
    }
}

/* ===== 把单个字符以 JSON 字符串形式安全写出（中文原样写，控制字符转义） ===== */
static void fput_json_str(FILE *f, const char *s) {
    fputc('"', f);
    for (const unsigned char *p = (const unsigned char*)s; *p; ) {
        if (*p == '"')  { fputs("\\\"", f); p++; }
        else if (*p == '\\') { fputs("\\\\", f); p++; }
        else if (*p == '\n') { fputs("\\n",  f); p++; }
        else if (*p == '\r') { fputs("\\r",  f); p++; }
        else if (*p == '\t') { fputs("\\t",  f); p++; }
        else if (*p < 0x20)   { fprintf(f, "\\u%04x", *p); p++; }
        else if (*p < 0x80)   { fputc(*p, f); p++; }
        else {
            /* UTF-8 多字节原样输出（JSON 支持 UTF-8） */
            char ch[8]; int nb = utf8_decode(p, ch);
            fwrite(ch, 1, nb, f);
            p += nb;
        }
    }
    fputc('"', f);
}

/* ===== tokenize 一个文本文件，写出 uint16 二进制 ===== */
static long tokenize_file(const char *txt_path, const char *bin_path, Dict *stoi) {
    FILE *ft = fopen(txt_path, "rb");
    if (!ft) { perror(txt_path); return 0; }
    FILE *fb = fopen(bin_path, "wb");
    if (!fb) { perror(bin_path); fclose(ft); return 0; }

    fseek(ft, 0, SEEK_END); long sz = ftell(ft); fseek(ft, 0, SEEK_SET);
    unsigned char *buf = (unsigned char*)malloc(sz + 16);
    size_t rd = fread(buf, 1, sz, ft); (void)rd; buf[sz] = 0;
    fclose(ft);

    const char *EOT = "<|endoftext|>"; size_t EOT_LEN = strlen(EOT);
    size_t n_tokens = 0;
    size_t i = 0;
    char ch[8];
    while (i < (size_t)sz) {
        /* 特殊 token 识别 */
        if (i + EOT_LEN <= (size_t)sz && memcmp(buf + i, EOT, EOT_LEN) == 0) {
            uint16_t id = EOT_ID;
            fwrite(&id, 2, 1, fb); n_tokens++;
            i += EOT_LEN;
            continue;
        }
        int nb = utf8_decode(buf + i, ch);
        int id = dict_get(stoi, ch);
        if (id < 0) id = UNK_ID;
        uint16_t u = (uint16_t)id;
        fwrite(&u, 2, 1, fb); n_tokens++;
        i += nb;
    }

    free(buf);
    fclose(fb);
    return (long)n_tokens;
}

/* ===== 主流程 ===== */
int main(int argc, char **argv) {
    parse_args(argc, argv);

    printf("========================================\n");
    printf("  C语言 字符级 Tokenizer 构建\n");
    printf("========================================\n");
    printf("  文本目录   : %s\n", g_text_dir);
    printf("  输出目录   : %s\n", g_out_dir);
    printf("  词表文件   : %s\n", g_tokenizer_path);
    printf("  最大词表   : %d\n\n", g_max_vocab);

    char train_txt[4096], eval_txt[4096];
    snprintf(train_txt, sizeof(train_txt), "%s/pretrain/train.txt", g_text_dir);
    snprintf(eval_txt,  sizeof(eval_txt),  "%s/eval/eval.txt",     g_text_dir);

    /* ---- 1. 统计字符频次 ---- */
    printf("[1/4] 统计 train.txt 字符频次...\n");
    Dict cnt; dict_init(&cnt, 4096);
    long train_chars = count_chars(train_txt, &cnt);
    long eval_chars  = count_chars(eval_txt,  &cnt);
    printf("  train 字符数: %ld\n", train_chars);
    printf("  eval  字符数: %ld\n", eval_chars);
    printf("  不同字符数  : %zu\n\n", cnt.size);

    /* ---- 2. 选取 top (max_vocab - N_SPECIAL) 字符 ---- */
    printf("[2/4] 构建词表 (按频次降序取 top %d)...\n", g_max_vocab - N_SPECIAL);
    CharCnt *items = (CharCnt*)malloc(cnt.size * sizeof(CharCnt));
    size_t n_items = 0;
    for (size_t b = 0; b < cnt.cap; b++) {
        if (cnt.keys[b]) {
            strncpy(items[n_items].ch, cnt.keys[b], 7); items[n_items].ch[7] = 0;
            items[n_items].cnt = cnt.vals[b];
            n_items++;
        }
    }
    qsort(items, n_items, sizeof(CharCnt), cmp_cnt_desc);
    int vocab_size = N_SPECIAL;
    size_t take = (size_t)(g_max_vocab - N_SPECIAL);
    if (take > n_items) take = n_items;
    vocab_size += (int)take;

    /* ---- 3. 构建 stoi 字典 + 写 tokenizer.json ---- */
    printf("[3/4] 写出 %s (vocab_size = %d)...\n", g_tokenizer_path, vocab_size);
    /* 建输出目录 */
    {
        char dir[4096];
        snprintf(dir, sizeof(dir), "%s", g_tokenizer_path);
        char *slash = strrchr(dir, '/');
        if (slash) { *slash = 0; } else { strcpy(dir, "."); }
        char cmd[8192];
        snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\" \"%s\"", dir, g_out_dir);
        (void)system(cmd);
    }

    Dict stoi; dict_init(&stoi, (size_t)vocab_size * 2);
    FILE *fj = fopen(g_tokenizer_path, "wb");
    if (!fj) { perror(g_tokenizer_path); return 1; }
    fputs("{\n", fj);
    fprintf(fj, "  \"vocab_size\": %d,\n", vocab_size);
    fputs(  "  \"special_tokens\": [", fj);
    for (int i = 0; i < N_SPECIAL; i++) {
        if (i) fputs(", ", fj);
        fput_json_str(fj, SPECIAL_TOKENS[i]);
    }
    fputs("],\n", fj);
    fputs(  "  \"itos\": [\n", fj);
    /* itos 顺序：special -> 按频次排序的字符 */
    for (int i = 0; i < N_SPECIAL; i++) {
        fputs("    ", fj); fput_json_str(fj, SPECIAL_TOKENS[i]);
        if (i != vocab_size - 1) fputc(',', fj);
        fputc('\n', fj);
        dict_put(&stoi, SPECIAL_TOKENS[i], i);
    }
    for (size_t i = 0; i < take; i++) {
        fputs("    ", fj); fput_json_str(fj, items[i].ch);
        int id = N_SPECIAL + (int)i;
        dict_put(&stoi, items[i].ch, id);
        if (id != vocab_size - 1) fputc(',', fj);
        fputc('\n', fj);
    }
    fputs("  ],\n", fj);
    /* stoi 反向：写出字典 */
    fputs("  \"stoi\": {\n", fj);
    int first = 1;
    for (size_t b = 0; b < stoi.cap; b++) {
        if (stoi.keys[b]) {
            if (!first) fputs(",\n", fj);
            first = 0;
            fputs("    ", fj);
            fput_json_str(fj, stoi.keys[b]);
            fprintf(fj, ": %d", stoi.vals[b]);
        }
    }
    fputs("\n  }\n", fj);
    fputs("}\n", fj);
    fclose(fj);

    /* ---- 4. Tokenize train/eval -> uint16 bin ---- */
    printf("[4/4] Tokenize 并写出 uint16 二进制文件...\n");
    char train_bin[4096], eval_bin[4096];
    snprintf(train_bin, sizeof(train_bin), "%s/train.bin", g_out_dir);
    snprintf(eval_bin,  sizeof(eval_bin),  "%s/eval.bin",  g_out_dir);
    long nt_tok = tokenize_file(train_txt, train_bin, &stoi);
    long ne_tok = tokenize_file(eval_txt,  eval_bin,  &stoi);

    printf("  train.bin -> %-40s %10ld tokens  (%ld bytes)\n",
           train_bin, nt_tok, nt_tok * 2);
    printf("  eval.bin  -> %-40s %10ld tokens  (%ld bytes)\n",
           eval_bin,  ne_tok, ne_tok * 2);
    printf("\n✅ Tokenizer 构建完成！\n");

    free(items);
    dict_free(&cnt);
    dict_free(&stoi);
    return 0;
}
