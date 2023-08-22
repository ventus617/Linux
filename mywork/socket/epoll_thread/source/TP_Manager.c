#include"thread_poll.h"

void* TP_Manager(void*arg)
{
	tp_t*tp=(tp_t*)arg;
	double State;
	int alive;
	int busy;
	int flag;
	int Max;
	int Min;
	int Thread_Cur=0;
	while(tp->Thread_Shutdown)
	{
		pthread_mutex_lock(&tp->var_lock);
		alive=tp->Thread_Alive;
		busy=tp->Thread_Busy;
		Max=tp->Thread_Max;
		Min=tp->Thread_Min;
		flag=tp->Thread_flag;
		pthread_mutex_unlock(&tp->var_lock);
		State=(double)busy/(alive+busy)*100;
		Thread_Cur=alive+busy;
		printf("Busy Thread Number:%d,Alive Thread Number:%d,State:%.2lf\n",busy,alive,State);
		while(flag)
		{	pthread_mutex_lock(&tp->var_lock);
			flag=tp->Thread_flag;
			pthread_mutex_unlock(&tp->var_lock);
			printf("Wait For Thread Exit,flag %d\n",flag);
			sleep(1);
		}
		//如果大于70就扩容
		if(State>=70&&(Thread_Cur+_INCREASE_THREAD)<Max)
		{
			printf("Increase ThreadNumber\n");
			pthread_mutex_lock(&tp->var_lock);
			tp->Thread_Alive+=_INCREASE_THREAD;
			for(int i=0;i<_INCREASE_THREAD;i++)
			{
				int err;
				if((err=pthread_create(&tp->cus_tids[i+Thread_Cur],&thread_attr,TP_Customer,(void*)tp))!=0)
				{
					printf("pthread create call failed:%s\n",strerror(err));
					exit(0);
				}
			}
			pthread_mutex_unlock(&tp->var_lock);
		}
		//如果闲置数量是繁忙的2倍或者闲置占比较高就缩减
		if((busy*2<=alive||State<=20)&&Thread_Cur-_THREAD_REDUCE>=Min)
		{
			pthread_mutex_lock(&tp->var_lock);
			tp->Thread_flag=_THREAD_REDUCE;
			pthread_mutex_unlock(&tp->var_lock);
			for(int i=0;i<_THREAD_REDUCE;i++)
			{
				pthread_cond_signal(&tp->Not_Full);
			}
			printf("Manager Thread 0x%x,Delete Old Thread Successfully..\n",    (unsigned int)pthread_self());

		}
		sleep(1);
	}
	return NULL;
}
