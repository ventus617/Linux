#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>


int var;
pthread_mutex_t lock1;
pthread_mutex_t lock2;

void *thread_task1(void*arg)
{
	sleep(0);
	while(1)
	{
		//申请锁1
		pthread_mutex_lock(&lock1);
		printf("thread0x%x working...var=%d\n",(unsigned int)pthread_self(),var++);
		//申请锁2
		pthread_mutex_lock(&lock2);
		//释放锁1
		pthread_mutex_unlock(&lock1);
		//释放锁2
		pthread_mutex_unlock(&lock2);
	}
}
void *thread_task2(void*arg)
{
	sleep(0);
	while(1)
	{
		//申请锁2
		pthread_mutex_lock(&lock2);
		printf("thread0x%x working...var=%d\n",(unsigned int)pthread_self(),var++);
		//申请锁1
		pthread_mutex_lock(&lock1);
		//释放锁2
		pthread_mutex_unlock(&lock2);
		//释放锁1
		pthread_mutex_unlock(&lock1);
	}
}


int main(void)
{
	pthread_mutex_init(&lock1,NULL);
	pthread_mutex_init(&lock2,NULL);
	pthread_t tid[2];
	pthread_create(&tid[0],NULL,thread_task1,NULL);
	pthread_create(&tid[1],NULL,thread_task2,NULL);
	while(1)
		sleep(1);
	return 0;
}
