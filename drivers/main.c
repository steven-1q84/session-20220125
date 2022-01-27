#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s_car; //汽车信号量：表示驾驶室是否有人
sem_t s_key; //钥匙信号量：表示车钥匙有没有被拿走

#define P(s) sem_wait(&s)
#define V(s) sem_post(&s)

/*
 * 正常司机线程函数
 **/
static void* sTaskNormalDriver(void *arg) {

    printf("正常司机%d准备出门\n", *((int *)arg));
    sleep(2);

    printf(
        "正常司机%d找钥匙: s_key == %d, P(s_key)\n"
    ,   *((int *)arg)
    ,   s_key.__align
    );
    P(s_key);
    usleep(100000);

    printf(
        "正常司机%d找车: s_car == %d，P(s_car)\n"
    ,   *((int *)arg)
    ,   s_car.__align
    );
    P(s_car);

    printf("正常司机%d开车...\n", *((int *)arg));
    sleep(2);

    printf("正常司机%d停车...\n", *((int *)arg));
    sleep(1);

    V(s_car);
    printf(
        "正常司机%d从车里出来: V(s_car), s_car == %d\n"
    ,   *((int *)arg)
    ,   s_car.__align
    );

    usleep(100000);

    V(s_key);
    printf(
        "正常司机%d还钥匙: V(s_key), s_key == %d\n"
    ,   *((int *)arg)
    ,   s_key.__align
    );
}

/*
 * 外星司机函数
 **/
static void* sTaskAlienDriver(void *arg) {
    printf("外星司机准备出门\n");
    sleep(2);

    if(
        arg != NULL
    &&  !strcmp((char *)arg, "educated")
    ) {
        printf("外星司机先找钥匙后找车\n");
        printf("外星司机找钥匙: s_key == %d, P(s_key)\n", s_key.__align);
        P(s_key);

        usleep(100000);

        printf("外星司机找车: s_car == %d，P(s_car)\n", s_car.__align);
        P(s_car);
    } else {
        printf("外星司机先找车后找钥匙\n");
        printf("外星司机找车: s_car == %d，P(s_car)\n", s_car.__align);
        P(s_car);

        usleep(100000);

        printf("外星司机找钥匙: s_key == %d, P(s_key)\n", s_key.__align);
        P(s_key);
    }

    printf("外星司机开车...\n");
    sleep(2);

    printf("外星司机停车...\n");
    sleep(1);

    if(
        arg != NULL
    &&  !strcmp((char *)arg, "educated")
    ) {
        V(s_car);
        printf("外星司机从车里出来: V(s_car), s_car == %d\n", s_car.__align);
    
        usleep(100000);
    
        V(s_key);
        printf("外星司机还钥匙: V(s_key), s_key == %d\n", s_key.__align);
    } else {
        V(s_key);
        printf("外星司机还钥匙: V(s_key), s_key == %d\n", s_key.__align);

        usleep(100000);

        V(s_car);
        printf("外星司机从车里出来: V(s_car), s_car == %d\n", s_car.__align);
    }
}

int main(int argc, char **argv) {

    if(
        argv[1] != NULL
    &&  !strcmp(argv[1], "ufo")
    ) {
        printf("混入了一只外星司机\n");

        if(
            argv[2] != NULL
        &&  !strcmp(argv[2], "educated")
        ) {
            printf("外星司机熟悉了地球生活\n");
        }
    }

    sem_init(&s_car, 0, 1); // 初始化汽车信号量为 1，表示汽车没人开
    sem_init(&s_key, 0, 1); // 初始化钥匙信号量为 1，表示钥匙没人用

    pthread_t tid_nd[3], tid_ad;
    int nd_id[3] = {1, 2, 3};

    pthread_create(  //创建正常司机1任务线程
        &tid_nd[0]
    ,   NULL
    ,   sTaskNormalDriver
    ,   &nd_id[0]
    );

    pthread_create(  //创建正常司机2任务线程
        &tid_nd[1]
    ,   NULL
    ,   sTaskNormalDriver
    ,   &nd_id[1]
    );

    pthread_create(  //创建正常司机3任务线程
        &tid_nd[2]
    ,   NULL
    ,   sTaskNormalDriver
    ,   &nd_id[2]
    );

    if(
        argv[1] != NULL
    &&  !strcmp((char *)argv[1], "ufo")
    ) {

        pthread_create(  //创建外星司机任务线程
            &tid_ad
        ,   NULL
        ,   sTaskAlienDriver
        ,   argv[2]
        );
    }

    for(int i = 0; i < 3; i ++) {
        pthread_join(tid_nd[i], NULL); //阻塞等待正常司机线程汇合
    }

    if (
        argv[1] != NULL
    &&  !strcmp((char *)argv[1], "ufo")
    ) {
        pthread_join(tid_ad, NULL); //阻塞等待外星司机线程汇合
    }

    return 0;
}


