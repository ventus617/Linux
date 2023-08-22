#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

typedef void*(*PFUN)(void*);
//产品任务结构体
typedef struct
{
	PFUN work;//任务地址
	void* arg;//任务参数
}task_t;

//任务队列结构体
typedef struct
{
	task_t* business_list;//任务列表
	int front;//队列头索引
	int rear;//队列尾索引
	int max;//最大任务数
	int cur;//当前任务数
}queue_task;

//队列初始化函数
queue_task* queue_init(int maxnum)
{
	queue_task*que=NULL;
	//给空间
	if((que=(queue_task*)malloc(sizeof(queue_task)))==NULL)
	{
		perror("malloc call failed");
		exit(0);
	}
	//给内部的任务列表空间
	if((que->business_list=(task_t*)malloc(sizeof(task_t)*maxnum))==NULL)
	{
		perror("malloc call failed");
		exit(0);
	}
	que->front=0;
	que->rear=0;
	que->max=maxnum;
	que->cur=0;
}

//锁和条件变量
pthread_mutex_t lock;
pthread_cond_t NotFull;
pthread_cond_t NotEmpty;
//消费者线程任务
void* pthread_task(void*arg)
{
	pthread_detach(pthread_self());
	queue_task*que=(queue_task*)arg;
	task_t bs;
	while(1)
	{
		pthread_mutex_lock(&lock);
		while(que->cur==0)
		{
			//消费者线程在空的时候挂起
			pthread_cond_wait(&NotEmpty,&lock);
		}
		//传类型结构体
		bs.work=que->business_list[que->rear].work;
		bs.arg=que->business_list[que->rear].arg;
		--(que->cur);
		que->rear=(que->rear+1)%que->max;
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&NotFull);
		//已经拿出任务了，应该先唤醒再执行
		(bs.work)(bs.arg);//任务执行应该放在队列的锁的外面
	}
}
//实际工作
void* business(void*arg)
{
	printf("pthread 0x%x working in it..cur=%d\n",(unsigned int)pthread_self(),*(int*)arg);
	sleep(1);
	return NULL;
}
//生产者线程
int main(void)
{
	if(pthread_mutex_init(&lock,NULL)!=0&&pthread_cond_init(&NotFull,NULL)&&pthread_cond_init(&NotEmpty,NULL)!=0)
	{
		printf("lock or cond init failed\n");
		exit(0);
	}
	pthread_t tid[10];//10个消费者
	queue_task*que=queue_init(30);
	int err;
	for(int i=0;i<10;i++)
	{
		//给消费者传递任务队列
		if((err=pthread_create(&tid[i],NULL,pthread_task,(void*)que))>0)
		{
			printf("pthread create failed %s\n",strerror(err));
			exit(0);
		}
	}
	//执行50次任务添加
	for(int i=0;i<50;i++)
	{
		//生产者生产任务
		pthread_mutex_lock(&lock);
		while(que->cur==que->max)
		{
			pthread_cond_wait(&NotFull,&lock);
		}
		task_t bs;
		bs.work=business;
		bs.arg=(int*)&que->cur;
		que->business_list[que->front].work=bs.work;
		que->business_list[que->front].arg=bs.arg;
		++(que->cur);
		que->front=(que->front+1)%que->max;//环形队列的偏移计算
		printf("pthread 0x %x add task cur=%d\n",(unsigned int)pthread_self(),que->cur);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&NotEmpty);
	}
	while(1)
		sleep(1);
	pthread_cond_destroy(&NotFull);
	pthread_cond_destroy(&NotEmpty);
	pthread_mutex_destroy(&lock);
	free(que->business_list);
	free(que);
	return 0;
}
