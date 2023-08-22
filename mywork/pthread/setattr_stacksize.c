#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

//设置地址
#define STACK_SIZE 0X100000
//针对X86修改栈大小是有效的
void* thread_task(void*arg)
{
	pthread_detach(pthread_self());
	while(1)
		sleep(1);
}
int main(void)
{
	pthread_attr_t new_attr;
	pthread_attr_init(&new_attr);
	size_t stacksize=STACK_SIZE;
	void*stackaddr;
	//创建线程
	int flag=0;
	int err;
	pthread_t tid;
	while(1)
	{
		//申请1M给stackaddr
		if((stackaddr=(void*)malloc(stacksize))==NULL)
		{
			perror("malloc call failed");
			exit(0);
		}
		pthread_attr_setstack(&new_attr,stackaddr,stacksize);
		if((err=pthread_create(&tid,&new_attr,thread_task,NULL))>0)
		{
			printf("create thread failed %s\n",strerror(err));
			exit(0);
		}
		printf("thread %d\n",++flag);
	}
	pthread_attr_destroy(&new_attr);
	return 0;
}
