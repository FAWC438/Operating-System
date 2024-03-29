#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>

/*
    切记链接上 -lpthread 库
*/

int avg_num, max_num, min_num, len = 0; // 题目要求的3个全局变量
int pthread_kill(pthread_t thread, int sig);

/**
 * @brief Get the Avg object
 * 
 * @param param 
 * @return void* 
 */
void *getAvg(void *param)
{
    int *seq = (int *)param, sum = 0;

    for (size_t i = 0; i < len; i++)
        sum += seq[i];

    avg_num = sum / len;

    pthread_exit(0);
}

/**
 * @brief Get the Max object
 * 
 * @param param 
 * @return void* 
 */
void *getMax(void *param)
{ /* the thread */

    int *seq = (int *)param, max_temp = INT_MIN;

    for (size_t i = 0; i < len; i++)
        if (seq[i] > max_temp)
            max_temp = seq[i];

    max_num = max_temp;

    pthread_exit(0);
}

/**
 * @brief Get the Min object
 * 
 * @param param 
 * @return void* 
 */
void *getMin(void *param)
{ /* the thread */

    int *seq = (int *)param, min_temp = INT_MAX;

    for (size_t i = 0; i < len; i++)
        if (seq[i] < min_temp)
            min_temp = seq[i];

    min_num = min_temp;

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int arr[1024], kill_rc;
    char c = '0';

    while (c != '\n')
    {
        scanf("%d", &arr[len]);
        c = getchar();
        len++;
    }

    pthread_t tid1, tid2, tid3;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // 线程1
    pthread_create(&tid1, &attr, getAvg, arr);
    pthread_join(tid1, NULL);
    /*
        pthread_kill()函数用于判断线程是否结束。
        这么做的原因是，题目要求必须在子线程结束后输出结果。
        尽管pthread_join()作用就是以阻塞的方式等待thread指定的线程结束，
        但是使用pthread_kill()能增加程序容错性和鲁棒性。
    */
    kill_rc = pthread_kill(tid1, 0);
    if (kill_rc == ESRCH)
        printf("The average value is %d\n", avg_num);
    else if (kill_rc == EINVAL)
        printf("signal is invalid\n");
    else
        printf("the thread 1 is still alive\n");

    // 线程2
    pthread_create(&tid2, &attr, getMin, arr);
    pthread_join(tid2, NULL);
    kill_rc = pthread_kill(tid2, 0);
    if (kill_rc == ESRCH)
        printf("The minimum value is %d\n", min_num);
    else if (kill_rc == EINVAL)
        printf("signal is invalid\n");
    else
        printf("the thread 2 is still alive\n");

    // 线程3
    pthread_create(&tid3, &attr, getMax, arr);
    pthread_join(tid3, NULL);
    kill_rc = pthread_kill(tid3, 0);
    if (kill_rc == ESRCH)
        printf("The maximum value is %d\n", max_num);
    else if (kill_rc == EINVAL)
        printf("signal is invalid\n");
    else
        printf("the thread 3 is still alive\n");
}