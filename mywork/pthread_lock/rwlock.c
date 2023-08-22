#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int var;
pthread_rwlock_t lock;
//写任务
void* thread_task1(void*arg)
{
	while(1)
	{
		pthread_rwlock_wrlock(&lock);
		printf("write thread 0x%x ++var=%d\n",(unsigned int)pthread_self(),++var);
		pthread_rwlock_unlock(&lock);
		usleep(100000);
	}
}
//读任务
void* thread_task2(void*arg)
{
	while(1)
	{
		pthread_rwlock_rdlock(&lock);
		printf("read thread 0x%x var=%d\n",(unsigned int)pthread_self(),var);
		pthread_rwlock_unlock(&lock);
		usleep(100000);
	}
}

int main(void)
{
	pthread_t tid[8];
	//初始化读写锁
	pthread_rwlock_init(&lock,NULL);
	int i;
	var=0;
	//创建2个写线程
	for(i=0;i<2;i++)
	{
		pthread_create(&tid[i],NULL,thread_task1,NULL);
	}
	//创建6个读线程
	for(i;i<8;i++)
	{
		pthread_create(&tid[i],NULL,thread_task2,NULL);
	}
	while(i--)
	{
		pthread_join(tid[i],NULL);
	}
	pthread_rwlock_destroy(&lock);
	return 0;
}
