#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>


void* thread_task1(void* arg)
{
	//普通线程任务
	printf("NTHREAD1 tid 0x%x Running...arg=%d\n",(unsigned int)pthread_self(),*(int*)arg);
	sleep(5);
	pthread_exit((void*)666);
}

void* thread_task2(void* arg)
{
	//普通线程任务
	printf("NTHREAD2 tid 0x%x Running...arg=%d\n",(unsigned int)pthread_self(),*(int*)arg);
	while(1)
		sleep(1);
}

int main(void)
{
	pthread_t tid[2];
	void*reval=NULL;
	int a=1024;
	pthread_create(&tid[0],NULL,thread_task1,(void*)&a);
	pthread_create(&tid[1],NULL,thread_task2,(void*)&a);
	printf("master thread tid 0x%x,Create Nthread tid 0x%x 0x%x\n",(unsigned int)pthread_self(),(unsigned int)tid[0],(unsigned int)tid[1]);
	pthread_join(tid[0],&reval);
	printf("master thread join success,exit code%ld\n",(long int)reval);
	while(1)
		sleep(1);
	return 0;
}
