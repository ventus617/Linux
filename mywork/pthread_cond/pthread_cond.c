#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

int daychange;//时间的状态,1是白天,0是夜晚
pthread_mutex_t lock;
pthread_cond_t day;//白天工作的条件
pthread_cond_t night;//夜晚工作的条件

void* DAY_WORK(void* arg)
{
	pthread_detach(pthread_self());
	while(1)
	{
		//申请互斥锁
		pthread_mutex_lock(&lock);
		//条件变量判断,如果此时是黑夜,就挂起线程
		while(daychange==0)
		{
			printf("now day is night,A is sleeping\n");
			pthread_cond_wait(&day,&lock);
		}
		//工作区间
		printf("A is busy working\n");
		printf("now day is night,A is offwork\n");
		daychange=0;
		//解锁
		pthread_mutex_unlock(&lock);
		//工作完唤醒夜间工作的
		pthread_cond_signal(&night);
	}
	pthread_exit(NULL);
}

void* NIGHT_WORK(void*arg)
{
	pthread_detach(pthread_self());
	while(1)
	{
		//申请互斥锁
		pthread_mutex_lock(&lock);
		//条件变量条件判断,如果是白天，就挂起自己
		while(daychange==1)
		{
			printf("now day is morning,B is sleeping\n");
			pthread_cond_wait(&night,&lock);
		}
		//工作区间
		printf("B is busy working\n");
		printf("now day is morning,B is offwork\n");
		daychange=1;
		//解锁
		pthread_mutex_unlock(&lock);
		//工作完唤醒白天工作的
		pthread_cond_signal(&day);
	}
	pthread_exit(NULL);
}

int main(void)
{
	pthread_cond_init(&day,NULL);
	pthread_cond_init(&night,NULL);
	pthread_mutex_init(&lock,NULL);
	pthread_t tid[2];
	pthread_create(&tid[0],NULL,DAY_WORK,NULL);
	pthread_create(&tid[1],NULL,NIGHT_WORK,NULL);
	while(1)
		sleep(1);
	pthread_cond_destroy(&day);
	pthread_cond_destroy(&night);
	pthread_mutex_destroy(&lock);
	return 0;
}
