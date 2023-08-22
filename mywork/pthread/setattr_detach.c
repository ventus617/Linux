#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>


void *  thread_task(void* arg)
{
	printf("nthread tid %x\n",(unsigned int)pthread_self());
	while(1)
		sleep(1);
	return NULL;
}


int main(void)
{
	//定义属性
	pthread_attr_t newattr;
	//初始化属性
	pthread_attr_init(&newattr);
	//设置分离态
	int detachstate;
	pthread_attr_setdetachstate(&newattr,PTHREAD_CREATE_DETACHED);
	//获取退出状态
	pthread_attr_getdetachstate(&newattr,&detachstate);
	if((detachstate==PTHREAD_CREATE_DETACHED))
	{
		printf("newattr state is detached\n");
	}
	else
	{
		printf("newattr state is joinable\n");
	}
	//创建线程
	pthread_t tid;
	pthread_create(&tid,&newattr,thread_task,NULL);
	int err;
	//如果是分离态则进入错误处理,如果是回收态就正常阻塞
	if((err=pthread_join(tid,NULL))>0)
	{
		printf("thread_join failed:%s \n",strerror(err));
		pthread_attr_destroy(&newattr);
		exit(0);
	}
	pthread_attr_destroy(&newattr);
	return 0;
}
