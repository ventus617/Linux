#include"thread_poll.h"

void* TP_Customer(void*arg)
{
	tp_t*tp=(tp_t*)arg;
	while(tp->Thread_Shutdown)
	{
		pthread_mutex_lock(&tp->queue_lock);
		while(tp->Cur==0)
		{
			pthread_cond_wait(&tp->Not_Full,&tp->queue_lock);
			pthread_mutex_lock(&tp->var_lock);
			if(tp->Thread_flag>0)
			{
				pthread_mutex_unlock(&tp->queue_lock);
				tp->Thread_flag--;
				tp->Thread_Alive--;
				pthread_mutex_unlock(&tp->var_lock);
				pthread_exit(NULL);
			}
			pthread_mutex_unlock(&tp->var_lock);

		}
		if(tp->Thread_Shutdown!=1)
		{
			pthread_mutex_unlock(&tp->queue_lock);
			break;
		}
		//工作区间
		printf("GET WORK,0x%x is WOKING\n",(unsigned int)pthread_self());
		bs_t bs;
		bs.Business_Addr=tp->Container[tp->Rear].Business_Addr;
		bs.arg=tp->Container[tp->Rear].arg;
		tp->Cur--;
		tp->Rear=(tp->Rear+1)%tp->Max;
		pthread_cond_signal(&tp->Not_Empty);
		pthread_mutex_unlock(&tp->queue_lock);
		pthread_mutex_lock(&tp->var_lock);
		tp->Thread_Busy++;
		tp->Thread_Alive--;
		pthread_mutex_unlock(&tp->var_lock);
		bs.Business_Addr(bs.arg);
		pthread_mutex_lock(&tp->var_lock);
		tp->Thread_Busy--;
		tp->Thread_Alive++;
		pthread_mutex_unlock(&tp->var_lock);
	}
	printf("Customer Thread 0x%x Shutdown\n",(unsigned int)pthread_self());
	pthread_exit(NULL);
}
