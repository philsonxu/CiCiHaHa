/**
 * ============================================================================
 * C语言哈希表实现（链地址法解决冲突）
 * 编译：gcc -Wall -std=c99 -o hash_table hash_table.c
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75f

/* 键值对节点 */
typedef struct hash_entry {
    char *key;
    void *value;
    struct hash_entry *next;
} hash_entry_t;

/* 哈希表结构 */
typedef struct hash_table {
    hash_entry_t **buckets;    // 桶数组
    size_t size;               // 当前元素个数
    size_t capacity;           // 桶数量
} hash_table_t;

/* FNV-1a 哈希算法：分布均匀、计算快 */
static uint32_t hash_func(const char *key) {
    uint32_t hash = 2166136261u;
    while (*key) {
        hash ^= (unsigned char)*key++;
        hash *= 16777619u;
    }
    return hash;
}

/**
 * 创建哈希表
 */
hash_table_t* ht_create(size_t capacity) {
    hash_table_t *ht = (hash_table_t *)malloc(sizeof(hash_table_t));
    if (!ht) return NULL;

    ht->capacity = capacity > 0 ? capacity : INITIAL_CAPACITY;
    ht->size = 0;
    ht->buckets = (hash_entry_t **)calloc(ht->capacity, sizeof(hash_entry_t *));
    return ht;
}

/**
 * 扩容并重新哈希（负载因子超阈值时调用）
 */
static void ht_resize(hash_table_t *ht) {
    size_t new_cap = ht->capacity * 2;
    hash_entry_t **new_buckets = (hash_entry_t **)calloc(new_cap, sizeof(hash_entry_t *));

    for (size_t i = 0; i < ht->capacity; i++) {
        hash_entry_t *entry = ht->buckets[i];
        while (entry) {
            hash_entry_t *next = entry->next;
            uint32_t idx = hash_func(entry->key) % new_cap;
            entry->next = new_buckets[idx];
            new_buckets[idx] = entry;
            entry = next;
        }
    }

    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->capacity = new_cap;
}

/**
 * 插入键值对（key存在则覆盖value）
 */
void ht_put(hash_table_t *ht, const char *key, void *value) {
    if ((float)(ht->size + 1) / ht->capacity > LOAD_FACTOR_THRESHOLD) {
        ht_resize(ht);
    }

    uint32_t idx = hash_func(key) % ht->capacity;
    hash_entry_t *entry = ht->buckets[idx];

    // 查找是否已存在
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    // 不存在：头插法
    hash_entry_t *new_entry = (hash_entry_t *)malloc(sizeof(hash_entry_t));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = ht->buckets[idx];
    ht->buckets[idx] = new_entry;
    ht->size++;
}

/**
 * 查找
 */
void* ht_get(hash_table_t *ht, const char *key) {
    uint32_t idx = hash_func(key) % ht->capacity;
    hash_entry_t *entry = ht->buckets[idx];
    while (entry) {
        if (strcmp(entry->key, key) == 0) return entry->value;
        entry = entry->next;
    }
    return NULL;
}

/**
 * 删除
 */
int ht_remove(hash_table_t *ht, const char *key) {
    uint32_t idx = hash_func(key) % ht->capacity;
    hash_entry_t *entry = ht->buckets[idx];
    hash_entry_t *prev = NULL;

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) prev->next = entry->next;
            else ht->buckets[idx] = entry->next;
            free(entry->key);
            free(entry);
            ht->size--;
            return 1;
        }
        prev = entry;
        entry = entry->next;
    }
    return 0;
}

/**
 * 遍历
 */
void ht_foreach(hash_table_t *ht, void (*visit)(const char*, void*)) {
    for (size_t i = 0; i < ht->capacity; i++) {
        hash_entry_t *entry = ht->buckets[i];
        while (entry) {
            visit(entry->key, entry->value);
            entry = entry->next;
        }
    }
}

/**
 * 销毁
 */
void ht_destroy(hash_table_t *ht) {
    for (size_t i = 0; i < ht->capacity; i++) {
        hash_entry_t *entry = ht->buckets[i];
        while (entry) {
            hash_entry_t *tmp = entry;
            entry = entry->next;
            free(tmp->key);
            free(tmp);
        }
    }
    free(ht->buckets);
    free(ht);
}

/* ==========================================================================
 * 演示：单词计数器
 * ========================================================================== */
void print_entry(const char *key, void *val) {
    printf("  %-12s : %d\n", key, *(int *)val);
}

int main() {
    printf("=======================================\n");
    printf("   C语言哈希表：单词计数               \n");
    printf("=======================================\n\n");

    hash_table_t *ht = ht_create(4);  // 初始容量小，测试扩容

    // 模拟词频统计
    const char *words[] = {
        "hello", "world", "c", "language", "hello", "c",
        "advanced", "programming", "c", "hello", "world", "code"
    };
    int word_count = sizeof(words) / sizeof(words[0]);

    printf("统计词频（共 %d 个单词）：\n", word_count);
    for (int i = 0; i < word_count; i++) {
        int *count = (int *)ht_get(ht, words[i]);
        if (count) {
            (*count)++;
        } else {
            int *new_count = (int *)malloc(sizeof(int));
            *new_count = 1;
            ht_put(ht, words[i], new_count);
        }
    }

    printf("哈希表元素数: %zu, 桶数: %zu\n", ht->size, ht->capacity);
    printf("\n词频统计结果：\n");
    ht_foreach(ht, print_entry);

    // 查找测试
    printf("\n查找 'c' 出现次数：%d\n", *(int *)ht_get(ht, "c"));
    printf("查找 'python'：%s\n", ht_get(ht, "python") ? "存在" : "不存在");

    // 删除测试
    ht_remove(ht, "world");
    printf("删除 'world' 后元素数：%zu\n", ht->size);

    // 释放 value 内存
    // （此处简化，实际应遍历释放）

    ht_destroy(ht);
    printf("\n哈希表已销毁。\n");
    return 0;
}
