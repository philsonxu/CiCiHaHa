/**
 * ============================================================================
 * C语言进程间通信（IPC）：共享内存 + 信号量同步
 * 说明：共享内存是最快的IPC方式，两个进程直接映射同一块物理内存
 * 编译：gcc -Wall -std=c99 -o shared_memory shared_memory.c -lpthread -lrt
 * 使用：先在一个终端运行 ./shared_memory w，再在另一个终端运行 ./shared_memory r
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <errno.h>

#define SHM_KEY   0x1234
#define SEM_KEY   0x5678
#define SHM_SIZE  1024

/* 信号量操作：P（等待）和 V（信号） */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int sem_p(int sem_id) {    // P操作：获取资源
    struct sembuf sb = {0, -1, 0};
    return semop(sem_id, &sb, 1);
}

int sem_v(int sem_id) {    // V操作：释放资源
    struct sembuf sb = {0, 1, 0};
    return semop(sem_id, &sb, 1);
}

int main(int argc, char *argv[]) {
    printf("=======================================\n");
    printf("   共享内存 + 信号量 IPC 演示          \n");
    printf("=======================================\n\n");

    if (argc != 2 || (argv[1][0] != 'w' && argv[1][0] != 'r')) {
        printf("用法: %s w  (写端)\n      %s r  (读端)\n", argv[0], argv[0]);
        return 1;
    }

    // 1. 创建/获取共享内存
    int shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id < 0) { perror("shmget"); return 1; }
    char *shm_ptr = (char *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (char *)-1) { perror("shmat"); return 1; }

    // 2. 创建/获取信号量（初始值为0，写端写完再V）
    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id < 0) { perror("semget"); return 1; }

    if (argv[1][0] == 'w') {
        // 写端：初始化信号量为0
        union semun arg;
        arg.val = 0;
        semctl(sem_id, 0, SETVAL, arg);

        printf("[写端] 请输入要发送的消息（输入 exit 退出）：\n");
        while (1) {
            char input[256];
            printf("> ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;

            strncpy(shm_ptr, input, SHM_SIZE - 1);
            sem_v(sem_id);  // 通知读端有数据

            if (strcmp(input, "exit") == 0) break;
        }
    } else {
        printf("[读端] 等待写端发送消息...\n");
        while (1) {
            sem_p(sem_id);  // 等待数据
            printf("收到消息: %s\n", shm_ptr);
            if (strcmp(shm_ptr, "exit") == 0) break;
        }
    }

    // 清理
    shmdt(shm_ptr);
    if (argv[1][0] == 'w') {
        shmctl(shm_id, IPC_RMID, NULL);
        semctl(sem_id, 0, IPC_RMID);
        printf("\nIPC 资源已清理\n");
    }

    printf("IPC 常见方式对比：\n");
    printf("  管道：       适合有亲缘关系的进程，字节流\n");
    printf("  命名管道：   任意进程间通信\n");
    printf("  消息队列：   按消息类型读取，有格式\n");
    printf("  共享内存：   最快，需要同步机制\n");
    printf("  信号量：     用于进程/线程同步，不传数据\n");
    printf("  套接字：     跨主机通信，最通用\n");

    return 0;
}
