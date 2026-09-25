/**
 * ============================================================================
 * C语言线程池实现
 * 原理：预先创建一组工作线程，任务入队后由空闲线程取出执行
 * 编译：gcc -Wall -std=c99 -o thread_pool thread_pool.c -lpthread
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

/* 任务节点 */
typedef struct task {
    void (*func)(void *arg);    // 任务函数
    void *arg;                   // 任务参数
    struct task *next;
} task_t;

/* 线程池结构 */
typedef struct thread_pool {
    pthread_t *threads;          // 工作线程数组
    int thread_count;            // 线程数量

    task_t *task_queue_head;     // 任务队列头
    task_t *task_queue_tail;     // 任务队列尾
    int task_count;              // 当前任务数

    pthread_mutex_t lock;        // 互斥锁
    pthread_cond_t  not_empty;   // 非空条件变量
    pthread_cond_t  not_full;    // 非满条件变量（可选）

    int shutdown;                // 关闭标志
    int max_tasks;               // 队列最大任务数
} thread_pool_t;

/* 工作线程函数 */
void* worker(void *arg) {
    thread_pool_t *pool = (thread_pool_t *)arg;

    while (1) {
        pthread_mutex_lock(&pool->lock);

        // 等待任务到来或关闭信号
        while (pool->task_count == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->not_empty, &pool->lock);
        }

        // 如果关闭且无任务，退出
        if (pool->shutdown && pool->task_count == 0) {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        // 从队列头取任务
        task_t *task = pool->task_queue_head;
        pool->task_queue_head = task->next;
        if (pool->task_queue_head == NULL) {
            pool->task_queue_tail = NULL;
        }
        pool->task_count--;

        pthread_cond_signal(&pool->not_full);
        pthread_mutex_unlock(&pool->lock);

        // 执行任务（锁外执行，提高并发）
        task->func(task->arg);
        free(task);
    }
    return NULL;
}

/**
 * 创建线程池
 */
thread_pool_t* thread_pool_create(int thread_count, int max_tasks) {
    thread_pool_t *pool = (thread_pool_t *)malloc(sizeof(thread_pool_t));
    if (!pool) return NULL;

    pool->thread_count = thread_count;
    pool->max_tasks = max_tasks;
    pool->task_count = 0;
    pool->shutdown = 0;
    pool->task_queue_head = pool->task_queue_tail = NULL;

    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->not_empty, NULL);
    pthread_cond_init(&pool->not_full, NULL);

    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&pool->threads[i], NULL, worker, pool);
        printf("工作线程 %lu 已启动\n", (unsigned long)pool->threads[i]);
    }

    return pool;
}

/**
 * 向线程池添加任务
 */
int thread_pool_add_task(thread_pool_t *pool, void (*func)(void *), void *arg) {
    pthread_mutex_lock(&pool->lock);

    // 队列满则等待
    while (pool->task_count >= pool->max_tasks && !pool->shutdown) {
        pthread_cond_wait(&pool->not_full, &pool->lock);
    }

    if (pool->shutdown) {
        pthread_mutex_unlock(&pool->lock);
        return -1;
    }

    task_t *task = (task_t *)malloc(sizeof(task_t));
    task->func = func;
    task->arg = arg;
    task->next = NULL;

    // 加入队尾
    if (pool->task_queue_tail) {
        pool->task_queue_tail->next = task;
    } else {
        pool->task_queue_head = task;
    }
    pool->task_queue_tail = task;
    pool->task_count++;

    pthread_cond_signal(&pool->not_empty);
    pthread_mutex_unlock(&pool->lock);
    return 0;
}

/**
 * 销毁线程池
 */
void thread_pool_destroy(thread_pool_t *pool) {
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->not_empty);
    pthread_mutex_unlock(&pool->lock);

    // 等待所有线程退出
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
        printf("工作线程 %lu 已退出\n", (unsigned long)pool->threads[i]);
    }

    // 清理剩余任务
    task_t *curr = pool->task_queue_head;
    while (curr) {
        task_t *tmp = curr;
        curr = curr->next;
        free(tmp);
    }

    free(pool->threads);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->not_empty);
    pthread_cond_destroy(&pool->not_full);
    free(pool);
}

/* ==========================================================================
 * 演示：模拟任务处理
 * ========================================================================== */
void example_task(void *arg) {
    int task_id = *(int *)arg;
    printf("[线程 %lu] 正在处理任务 %d...\n",
           (unsigned long)pthread_self(), task_id);
    usleep(100000 * (task_id % 3 + 1));  // 模拟耗时操作
    printf("[线程 %lu] 任务 %d 处理完成\n",
           (unsigned long)pthread_self(), task_id);
    free(arg);
}

int main() {
    printf("=======================================\n");
    printf("   C语言线程池演示                     \n");
    printf("=======================================\n\n");

    // 创建含3个工作线程、最多100个等待任务的线程池
    thread_pool_t *pool = thread_pool_create(3, 100);
    printf("\n线程池创建完毕（3个工作线程），开始提交10个任务...\n\n");

    // 提交10个任务
    for (int i = 1; i <= 10; i++) {
        int *task_id = (int *)malloc(sizeof(int));
        *task_id = i;
        thread_pool_add_task(pool, example_task, task_id);
    }

    // 等待所有任务处理完
    sleep(3);

    printf("\n所有任务提交完毕，关闭线程池...\n");
    thread_pool_destroy(pool);
    printf("\n线程池已销毁。\n");
    printf("\n线程池的应用场景：\n");
    printf("  1. Web 服务器处理客户端请求\n");
    printf("  2. 高并发任务处理（避免频繁创建/销毁线程）\n");
    printf("  3. 异步日志、数据库连接池\n");

    return 0;
}
