#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
void* pthread_task(void* arg)
{
	printf("%ld\n",(long int)arg);
	return NULL;
}

int main(void)
{
	pthread_t tid;
	int err;
	if((err=pthread_create(&tid,NULL,pthread_task,(void*)2))>0)
	{
		printf("pthread create failed %s\n",strerror(err));
		exit(0);
	}
	while(1)
		sleep(1);
	return 0;
}
