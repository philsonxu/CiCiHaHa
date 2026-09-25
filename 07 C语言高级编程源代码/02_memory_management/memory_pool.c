/**
 * ============================================================================
 * C语言内存管理：内存池实现
 * 说明：频繁 malloc/free 会造成内存碎片，内存池预先分配大块内存并按需切分
 * 编译：gcc -Wall -std=c99 -o memory_pool memory_pool.c
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* 内存块结构：每个空闲块用链表串联 */
typedef struct mem_block {
    size_t size;                // 块大小（不含头部）
    struct mem_block *next;     // 下一个空闲块
    int free;                   // 是否空闲
} mem_block_t;

/* 内存池结构 */
typedef struct mem_pool {
    void *memory;               // 预分配的内存起始地址
    size_t total_size;          // 内存池总大小
    mem_block_t *free_list;     // 空闲块链表头
} mem_pool_t;

/* 对齐到 8 字节 */
#define ALIGN8(size) (((size) + 7) & ~7)
#define BLOCK_HEADER_SIZE ALIGN8(sizeof(mem_block_t))

/**
 * 初始化内存池
 */
mem_pool_t* mem_pool_create(size_t pool_size) {
    mem_pool_t *pool = (mem_pool_t *)malloc(sizeof(mem_pool_t));
    if (!pool) return NULL;

    pool->memory = malloc(pool_size);
    if (!pool->memory) {
        free(pool);
        return NULL;
    }

    pool->total_size = pool_size;

    // 初始时整块都是空闲的
    pool->free_list = (mem_block_t *)pool->memory;
    pool->free_list->size = pool_size - BLOCK_HEADER_SIZE;
    pool->free_list->next = NULL;
    pool->free_list->free = 1;

    return pool;
}

/**
 * 内存池分配（首次适应算法）
 */
void* mem_pool_alloc(mem_pool_t *pool, size_t size) {
    if (!pool || size == 0) return NULL;

    size = ALIGN8(size);
    mem_block_t *curr = pool->free_list;
    mem_block_t *prev = NULL;

    while (curr) {
        if (curr->free && curr->size >= size) {
            // 如果剩余空间足够再分一个块，就拆分
            if (curr->size >= size + BLOCK_HEADER_SIZE + 8) {
                mem_block_t *new_block = (mem_block_t *)((char *)curr + BLOCK_HEADER_SIZE + size);
                new_block->size = curr->size - size - BLOCK_HEADER_SIZE;
                new_block->free = 1;
                new_block->next = curr->next;

                curr->size = size;
                curr->free = 0;
                curr->next = new_block;
            } else {
                // 剩余空间太小，全部分配
                curr->free = 0;
            }

            // 从空闲链表移除
            if (prev) {
                prev->next = curr->next;
            } else {
                pool->free_list = curr->next;
            }

            return (char *)curr + BLOCK_HEADER_SIZE;
        }
        prev = curr;
        curr = curr->next;
    }

    // 内存池不足
    return NULL;
}

/**
 * 内存池释放（合并相邻空闲块）
 */
void mem_pool_free(mem_pool_t *pool, void *ptr) {
    if (!pool || !ptr) return;

    mem_block_t *block = (mem_block_t *)((char *)ptr - BLOCK_HEADER_SIZE);
    block->free = 1;

    // 插入空闲链表头部
    block->next = pool->free_list;
    pool->free_list = block;

    // 简单合并：遍历空闲链表，合并地址相邻的块
    mem_block_t *curr = pool->free_list;
    while (curr && curr->next) {
        char *curr_end = (char *)curr + BLOCK_HEADER_SIZE + curr->size;
        if (curr_end == (char *)curr->next && curr->free && curr->next->free) {
            curr->size += BLOCK_HEADER_SIZE + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

/**
 * 销毁内存池
 */
void mem_pool_destroy(mem_pool_t *pool) {
    if (pool) {
        free(pool->memory);
        free(pool);
    }
}

/**
 * 打印内存池状态（调试用）
 */
void mem_pool_status(mem_pool_t *pool) {
    printf("内存池总大小: %zu 字节\n", pool->total_size);
    mem_block_t *curr = (mem_block_t *)pool->memory;
    int idx = 0;
    while ((char *)curr < (char *)pool->memory + pool->total_size) {
        printf("  块[%d]: 地址=%p 大小=%5zu %s\n",
               idx++, (void*)((char*)curr + BLOCK_HEADER_SIZE),
               curr->size, curr->free ? "[空闲]" : "[已分配]");
        curr = (mem_block_t *)((char *)curr + BLOCK_HEADER_SIZE + curr->size);
    }
}

/* ==========================================================================
 * 演示：对比 malloc vs 内存池
 * ========================================================================== */
int main() {
    printf("=======================================\n");
    printf("   C语言内存池实现与演示               \n");
    printf("=======================================\n\n");

    // 创建 1KB 内存池
    const size_t POOL_SIZE = 1024;
    mem_pool_t *pool = mem_pool_create(POOL_SIZE);
    if (!pool) {
        printf("内存池创建失败！\n");
        return -1;
    }

    printf("--- 初始状态 ---\n");
    mem_pool_status(pool);

    printf("\n--- 分配 128 字节（p1）---\n");
    int *p1 = (int *)mem_pool_alloc(pool, 128);
    for (int i = 0; i < 32; i++) p1[i] = i;
    mem_pool_status(pool);

    printf("\n--- 分配 256 字节（p2）---\n");
    char *p2 = (char *)mem_pool_alloc(pool, 256);
    strcpy(p2, "Hello Memory Pool!");
    printf("p2 内容: %s\n", p2);
    mem_pool_status(pool);

    printf("\n--- 分配 64 字节（p3）---\n");
    double *p3 = (double *)mem_pool_alloc(pool, 64);
    *p3 = 3.14159;
    printf("*p3 = %f\n", *p3);
    mem_pool_status(pool);

    printf("\n--- 释放 p2 ---\n");
    mem_pool_free(pool, p2);
    mem_pool_status(pool);

    printf("\n--- 释放 p1（触发合并）---\n");
    mem_pool_free(pool, p1);
    mem_pool_status(pool);

    printf("\n--- 重新分配 300 字节（验证合并后可分配）---\n");
    void *p4 = mem_pool_alloc(pool, 300);
    printf("p4 = %p %s\n", p4, p4 ? "分配成功" : "分配失败");
    mem_pool_status(pool);

    mem_pool_free(pool, p3);
    mem_pool_free(pool, p4);

    mem_pool_destroy(pool);
    printf("\n内存池已销毁。内存池相比 malloc 的优势：\n");
    printf("  1. 减少系统调用次数，分配/释放速度快\n");
    printf("  2. 减少内存碎片\n");
    printf("  3. 内存生命周期可统一管理\n");
    printf("  4. 适用于高频分配场景（如网络服务器、游戏引擎）\n");

    return 0;
}
