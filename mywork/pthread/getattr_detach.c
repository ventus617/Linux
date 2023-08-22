#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>


void* thread_task(void*arg)
{
	while(1)
	{
		pthread_testcancel();
		sleep(1);
	}
}


int main(void)
{
	//定义属性
	pthread_attr_t newattr;
	//初始化属性
	pthread_attr_init(&newattr);
	//获取退出状态
	int detachstate;
	pthread_attr_getdetachstate(&newattr,&detachstate);
	if(detachstate==PTHREAD_CREATE_JOINABLE)
	{
		printf("newattr exit type =JOINABLE\n");
	}
	else
	{
		printf("newattr exit type =DETACHED\n");
	}
	//获取栈的大小
	size_t stacksize;
	void* stackaddr;
	pthread_attr_getstack(&newattr,&stackaddr,&stacksize);
	printf("newattr stacksize:[%d] stackaddr:[%p]\n",(int)stacksize,stackaddr);
	//销毁属性
	pthread_attr_destroy(&newattr);
	return 0;
}

