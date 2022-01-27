#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s_wc; // 卫生间信号量：表示卫生间是否被占用

#define P(s) sem_wait(&s)
#define V(s) sem_post(&s)

/*
 * 丈夫线程函数
 **/
static void* sTaskHusbend(void *arg) {
    printf("老公起床\n");
    sleep(5);

    while(1) {
        printf("老公问洗手间里有没有人: s_wc == %d, P(s_wc)\n", s_wc.__align);
        P(s_wc);

        printf("老公使用洗手间...\n");
        sleep(3);

        V(s_wc);
        printf("老公用完洗手间: V(s_wc), s_wc == %d\n", s_wc.__align);
    }

    printf("老公出门上班\n");
}

/*
 * 妻子线程函数
 **/
static void* sTaskWife(void *arg) {
    printf("老婆起床\n");
    sleep(3);

    while(1) {
        printf("老婆问洗手间里有没有人: s_wc == %d, P(s_wc)\n", s_wc.__align);
        P(s_wc);

        printf("老婆使用洗手间...\n");
        sleep(8);

        V(s_wc);
        printf("老婆用完厕所: V(s_wc), s_wc == %d\n", s_wc.__align);
    }

    printf("老婆出门上班\n");
}

int main(void) {

    sem_init(&s_wc, 0, 1); // 初始化洗手间信号量

    pthread_t tid_h, tid_w;
    pthread_create(&tid_h, NULL, sTaskHusbend, NULL); //创建老公任务线程
    pthread_create(&tid_w, NULL, sTaskWife, NULL); //创建老婆任务线程

    pthread_join(tid_h, NULL); //阻塞等待老公线程汇合
    pthread_join(tid_w, NULL); //阻塞等待老婆线程汇合

    return 0;
}


