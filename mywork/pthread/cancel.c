#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>

void* thread_task(void*arg)
{
	//如果没有产生取消事件，就不会被取消
	while(1)
	{	
		printf("nthread 0x%x wait for cancel\n",(unsigned int)pthread_self());
		pthread_testcancel();
	}
	//无法终结
	pthread_exit((void*)666);
}


int main(void)
{
	pthread_t tid;
	pthread_create(&tid,NULL,thread_task,NULL);
	printf("master thread tid 0x%x,create nthread tid 0x%x\n",(unsigned int)pthread_self(),(unsigned int)tid);
	sleep(4);
	//发送线程取消函数
	pthread_cancel(tid);
	int err;
	void* reval=NULL;
	if((err=pthread_join(tid,&reval))>0)
	{
		printf("join failed:%s\n",strerror(err));
		exit(0);
	}
	printf("master thread_join success,exit code %ld\n",(long int)reval);
	return 0;
}


