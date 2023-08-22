#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<pthread.h>
#include<string.h>

//普通线程工作
void* pthread_task(void* arg)
{
	long int a=*(long int*)arg;
	printf("pthread id:%ld\n",a);
	return NULL;
}

//主线程工作
int main(void)
{
	for(long int i=1;i;i++)
	{
		pthread_t tid;
		int err=pthread_create(&tid,NULL,pthread_task,(void*)&i);
		if(err!=0)
		{
			printf("create pthread failed:%s\n",strerror(err));
			exit(0);
		}
	}
	return 0;
}
