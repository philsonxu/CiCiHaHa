/**
 * ============================================================================
 * C语言设计模式：观察者模式（发布-订阅）+ 状态机 + 策略模式
 * 编译：gcc -Wall -std=c99 -o observer_pattern observer_pattern.c
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================================================
 * 1. 观察者模式（Observer Pattern）
 *    事件源维护一组观察者，事件发生时逐个通知
 * ========================================================================== */
#define MAX_OBSERVERS 16

/* 观察者回调函数类型 */
typedef void (*ObserverCallback)(const char *event, void *data, void *user_data);

typedef struct Observer {
    ObserverCallback callback;
    void *user_data;   // 用户上下文
} Observer;

typedef struct EventEmitter {
    Observer observers[MAX_OBSERVERS];
    int count;
} EventEmitter;

/* 创建事件发布者 */
void emitter_init(EventEmitter *em) {
    em->count = 0;
}

/* 订阅事件 */
int emitter_on(EventEmitter *em, ObserverCallback cb, void *user_data) {
    if (em->count >= MAX_OBSERVERS) return -1;
    em->observers[em->count].callback = cb;
    em->observers[em->count].user_data = user_data;
    return em->count++;
}

/* 发布事件（通知所有观察者） */
void emitter_emit(EventEmitter *em, const char *event, void *data) {
    for (int i = 0; i < em->count; i++) {
        em->observers[i].callback(event, data, em->observers[i].user_data);
    }
}

/* 具体观察者：日志观察者 */
void logger_observer(const char *event, void *data, void *user_data) {
    (void)user_data;
    printf("  [日志] 收到事件 '%s', 数据=%s\n", event, (char *)data);
}

/* 具体观察者：告警观察者 */
void alert_observer(const char *event, void *data, void *user_data) {
    int *threshold = (int *)user_data;
    int value = atoi((char *)data);
    if (value > *threshold) {
        printf("  [告警] 事件 '%s' 值 %d 超过阈值 %d！\n", event, value, *threshold);
    }
}

/* ==========================================================================
 * 2. 策略模式（Strategy Pattern）：排序算法可替换
 * ========================================================================== */
typedef int (*CompareFunc)(const void *, const void *);

int cmp_int_asc(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}
int cmp_int_desc(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}
int cmp_int_abs(const void *a, const void *b) {
    int va = abs(*(int *)a), vb = abs(*(int *)b);
    return va - vb;
}

/* 通用冒泡排序，策略由参数传入 */
void sort_array(int *arr, int n, CompareFunc cmp) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (cmp(&arr[j], &arr[j+1]) > 0) {
                int t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;
            }
        }
    }
}

void print_array(const char *title, int *arr, int n) {
    printf("%s", title);
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

/* ==========================================================================
 * 3. 有限状态机（FSM）：用函数指针表实现
 * ========================================================================== */
typedef enum { STATE_IDLE, STATE_RUNNING, STATE_PAUSED, STATE_STOPPED, STATE_MAX } State;
typedef enum { EVENT_START, EVENT_PAUSE, EVENT_RESUME, EVENT_STOP } Event;

typedef State (*StateHandler)(Event event);

State state_idle(Event ev) {
    if (ev == EVENT_START)  { printf("  状态机：空闲 -> 运行中\n"); return STATE_RUNNING; }
    printf("  状态机：空闲态，忽略事件 %d\n", ev);
    return STATE_IDLE;
}
State state_running(Event ev) {
    if (ev == EVENT_PAUSE) { printf("  状态机：运行中 -> 暂停\n"); return STATE_PAUSED; }
    if (ev == EVENT_STOP)  { printf("  状态机：运行中 -> 已停止\n"); return STATE_STOPPED; }
    return STATE_RUNNING;
}
State state_paused(Event ev) {
    if (ev == EVENT_RESUME){ printf("  状态机：暂停 -> 运行中\n"); return STATE_RUNNING; }
    if (ev == EVENT_STOP)  { printf("  状态机：暂停 -> 已停止\n"); return STATE_STOPPED; }
    return STATE_PAUSED;
}
State state_stopped(Event ev) {
    (void)ev;
    return STATE_STOPPED;
}

/* 状态机表 */
StateHandler state_table[STATE_MAX] = {
    [STATE_IDLE] = state_idle,
    [STATE_RUNNING] = state_running,
    [STATE_PAUSED] = state_paused,
    [STATE_STOPPED] = state_stopped,
};

/* ==========================================================================
 * 主函数
 * ========================================================================== */
int main() {
    printf("=======================================\n");
    printf("   C语言设计模式演示                   \n");
    printf("=======================================\n");

    /* ---- 观察者模式演示 ---- */
    printf("\n===== 1. 观察者模式 =====\n");
    EventEmitter emitter;
    emitter_init(&emitter);
    int alert_threshold = 80;
    emitter_on(&emitter, logger_observer, NULL);
    emitter_on(&emitter, alert_observer, &alert_threshold);

    printf("发布 temperature 事件 60：\n");
    emitter_emit(&emitter, "temperature", "60");
    printf("发布 temperature 事件 90（触发告警）：\n");
    emitter_emit(&emitter, "temperature", "90");

    /* ---- 策略模式演示 ---- */
    printf("\n===== 2. 策略模式（排序） =====\n");
    int data[] = {-5, 3, -9, 1, 8, -2, 7, -4};
    int n = sizeof(data) / sizeof(data[0]);
    int buf[8];

    memcpy(buf, data, sizeof(data));
    sort_array(buf, n, cmp_int_asc);
    print_array("升序排序：  ", buf, n);

    memcpy(buf, data, sizeof(data));
    sort_array(buf, n, cmp_int_desc);
    print_array("降序排序：  ", buf, n);

    memcpy(buf, data, sizeof(data));
    sort_array(buf, n, cmp_int_abs);
    print_array("绝对值排序：", buf, n);

    /* ---- 状态机演示 ---- */
    printf("\n===== 3. 有限状态机（媒体播放器） =====\n");
    State current = STATE_IDLE;
    Event events[] = {EVENT_START, EVENT_PAUSE, EVENT_RESUME, EVENT_PAUSE, EVENT_STOP};
    for (int i = 0; i < 5; i++) {
        current = state_table[current](events[i]);
    }

    printf("\n设计模式在C中的本质：\n");
    printf("  - 回调函数 = 函数指针作为参数\n");
    printf("  - 策略模式 = 函数指针切换算法\n");
    printf("  - 观察者模式 = 函数指针数组 + 遍历调用\n");
    printf("  - 状态机     = 函数指针表 + 索引查表分发\n");
    printf("  - 工厂模式   = 根据类型ID返回不同实现\n");

    return 0;
}
