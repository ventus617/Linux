#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<pthread.h>

//A能写入缓存区A的 条件变量
pthread_cond_t A_IS_FULL;
//B能读取缓存区A写入缓存区B的条件变量
pthread_cond_t A_IS_NOT_FULL;
//C能读取缓存区B的条件变量
pthread_cond_t B_IS_FULL;
//缓冲区A的互斥锁
pthread_mutex_t lockA;
//缓冲区B的互斥锁
pthread_mutex_t lockB;
char bufferA[1024];
char bufferB[1024];
void* pthread_taskA(void*arg)
{
	pthread_detach(pthread_self());
	//打开日志文件
	FILE*fp;
	fp=fopen("ERROR.log","r");
	if(fp==NULL)
	{
		perror("fopen failed");
		exit(0);
	}
	bzero(bufferA,1024);
	while(1)
	{
		pthread_mutex_lock(&lockA);
		while(strlen(bufferA)!=0)
		{
			printf("A is working: %s\n",bufferA);
			pthread_cond_wait(&A_IS_FULL,&lockA);
		}
		//工作区间
		fgets(bufferA,sizeof(bufferA),fp);
		pthread_mutex_unlock(&lockA);
		pthread_cond_signal(&A_IS_NOT_FULL);
	}
	fclose(fp);
	return 0;
}
void* pthread_taskB(void*arg)
{
	pthread_detach(pthread_self());
	bzero(bufferB,1024);
	while(1)
	{
		//把A的缓存区读出来放在B
		pthread_mutex_lock(&lockA);
		while(strlen(bufferA)==0)
		{
			pthread_cond_wait(&A_IS_NOT_FULL,&lockA);
		}
		if((strstr(bufferA,"E CamX")!=NULL)||(strstr(bufferA,"E CHIUSECASE")!=NULL))
		{
			pthread_mutex_lock(&lockB);
			strcpy(bufferB,bufferA);
			pthread_cond_signal(&B_IS_FULL);
			pthread_mutex_unlock(&lockB);
		}
		bzero(bufferA,1024);
		pthread_mutex_unlock(&lockA);
		pthread_cond_signal(&A_IS_FULL);
	}
}
void* pthread_taskC(void*arg)
{
	pthread_detach(pthread_self());
	umask(0000);
	//打开文件
	int fd=open("result.log",O_CREAT|O_RDWR,0666);
	//当可用的时候读入缓存区
	while(1)
	{
		pthread_mutex_lock(&lockB);
		while(strlen(bufferB)==0)
		{
			pthread_cond_wait(&B_IS_FULL,&lockB);
		}
		//把B的数据写入result
		write(fd,bufferB,strlen(bufferB));
		printf("C is working %s\n",bufferB);
		bzero(bufferB,sizeof(bufferB));
		pthread_mutex_unlock(&lockB);
	}
	close(fd);
	return 0;
}
typedef void*(*PFUN)(void*);
int main(void)
{
	bzero(bufferA,1024);
	bzero(bufferB,1024);
	pthread_mutex_init(&lockA,NULL);
	pthread_mutex_init(&lockB,NULL);
	pthread_cond_init(&A_IS_FULL,NULL);
	pthread_cond_init(&B_IS_FULL,NULL);
	pthread_cond_init(&A_IS_NOT_FULL,NULL);
	pthread_t tid[3];
	PFUN thread_task[3];
	thread_task[0]=pthread_taskA;
	thread_task[1]=pthread_taskB;
	thread_task[2]=pthread_taskC;
	int err=0;
	for(int i=0;i<3;i++)
	{
		if((err=pthread_create(&tid[i],NULL,thread_task[i],NULL))>0)
		{
			printf("pthread create failed %s\n",strerror(err));
			exit(0);
		}
	}
	while(1)
		sleep(1);
	pthread_cond_destroy(&A_IS_FULL);
	pthread_cond_destroy(&A_IS_NOT_FULL);
	pthread_cond_destroy(&B_IS_FULL);
	pthread_mutex_destroy(&lockA);
	pthread_mutex_destroy(&lockB);
}

