#include"thread_poll.h"

tp_t* TP_Create(int Tmax,int Tmin,int Cmax)
{
	//初始化线程属性
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);
	//创建线程池类型
	tp_t *tp=(tp_t*)malloc(sizeof(tp_t));
	//初始化线程池资源
	pthread_cond_init(&tp->Not_Empty,NULL);
	pthread_cond_init(&tp->Not_Full,NULL);
	pthread_mutex_init(&tp->queue_lock,NULL);
	pthread_mutex_init(&tp->var_lock,NULL);
	tp->Thread_Max=Tmax;
	tp->Thread_Min=Tmin;
	tp->Thread_Shutdown=1;
	tp->Thread_flag=0;
	tp->Thread_Alive=Tmin;
	tp->Front=0;
	tp->Rear=0;
	tp->Cur=0;
	tp->Max=Cmax;
	tp->Container=(bs_t*)malloc(sizeof(bs_t)*Cmax);
	bzero(tp->Container,sizeof(bs_t)*Cmax);
	tp->cus_tids=(pthread_t*)malloc(sizeof(pthread_t)*Tmax);
	bzero(tp->cus_tids,sizeof(pthread_t)*Tmax);
	//预创建10个闲置消费者线程
	int err;
	for(int i=0;i<10;i++)
	{
		if((err=pthread_create(&tp->cus_tids[i],&thread_attr,TP_Customer,(void*)tp)!=0))
		{
			printf("create customer thread failed:%s\n",strerror(err));
			exit(0);
		}
	}
	//预创建1个管理者线程
	if((pthread_create(&tp->mag_tid,&thread_attr,TP_Manager,(void*)tp))!=0)
	{
		printf("create manager thread failed:%s\n",strerror(err));
		exit(0);
	}
	return tp;
}
