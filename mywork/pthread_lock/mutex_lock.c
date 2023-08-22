#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define FLAG 10000

int var;
pthread_mutex_t lock;
void* thread_task(void* arg)
{
	pthread_detach(pthread_self());
	int temp;
	for(int i=0;i<FLAG;i++)
	{
		//在使用区间内上锁，用完就解锁
		pthread_mutex_lock(&lock);
		temp=var;
		printf("thread 0x%x ++var=%d\n",(unsigned int)pthread_self(),temp++);
		var=temp;
		pthread_mutex_unlock(&lock);
	}
	pthread_exit((void*)666);
}

int main(void)
{
	var=1;
	//初始化锁
	pthread_mutex_init(&lock,NULL);
	//创建线程
	pthread_t tid[2];
	pthread_create(&tid[0],NULL,thread_task,NULL);
	pthread_create(&tid[1],NULL,thread_task,NULL);
	while(1)
		sleep(1);
	//回收锁
	pthread_mutex_destroy(&lock);
	return 0;
}
