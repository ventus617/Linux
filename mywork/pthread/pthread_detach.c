#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>



void* thread_task(void*arg)
{
	//普通线程任务
	pthread_detach(pthread_self());
	printf("Nthread1 tid 0x%x running..arg=%ld\n",(unsigned int)pthread_self(),(long int)arg);
	sleep(5);
	pthread_exit((void*)666);
}

int main(void)
{
	pthread_t tid;
	void * reval=NULL;
	int err;
	pthread_create(&tid,NULL,thread_task,(void*)1025);
	printf("master thread tid 0x%x,Create nthread tid 0x%x\n",(unsigned int)pthread_self(),(unsigned int)tid);
	//sleep(0);//放弃CPU,使得join回收不会先于分离态进行
	if((err=pthread_join(tid,&reval))>0)
	{
		printf("pthread join failed %s\n",strerror(err));
		exit(0);
	}
	printf("master thread join success,exit code%ld\n",(long int)reval);
	return 0;
}
