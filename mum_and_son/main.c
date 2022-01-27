#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s_son; // 儿子信号量：表示需不需要吃饭
sem_t s_mum; // 母亲信号量：表示饭做没做好

#define P(s) sem_wait(&s)
#define V(s) sem_post(&s)

/*
 * 儿子线程函数
 **/
static void* sTaskSon(void *arg) {
    sleep(5);

    V(s_son);
    printf("儿子饿了，叫妈妈做法: V(s_son), s_son == %d\n", s_son.__align);

    printf("儿子等待妈妈做好饭: s_mum == %d, P(s_mum)\n", s_mum.__align);
    P(s_mum);
      
    printf("儿子吃饭...\n");
    sleep(5);

    printf("儿子吃完饭\n");
}

/*
 * 母亲线程函数
 **/
static void* sTaskMum(void *arg) {
    printf("妈妈等待儿子的召唤: s_son == %d, P(s_son)\n", s_son.__align);
    P(s_son);

    printf("妈妈做饭...\n");
    sleep(5);

    V(s_mum);
    printf("妈妈做好饭叫儿子来吃: V(s_mum), s_mum == %d\n", s_mum.__align);
}

int main(void) {

    sem_init(&s_son, 0, 0); // 初始化儿子信号量为 0，表示不需要吃饭
    sem_init(&s_mum, 0, 0); // 初始化母亲信号量为 0，表示饭没做好

    pthread_t tid_son, tid_mum;
    pthread_create(&tid_son, NULL, sTaskSon, NULL); //创建儿子任务线程
    pthread_create(&tid_mum, NULL, sTaskMum, NULL); //创建母亲任务线程

    pthread_join(tid_son, NULL); //阻塞等待儿子线程汇合
    pthread_join(tid_mum, NULL); //阻塞等待母亲线程汇合

    return 0;
}


