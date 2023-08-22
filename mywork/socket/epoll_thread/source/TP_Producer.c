#include"thread_poll.h"

int TP_Producer(tp_t* tp,bs_t business)
{
	if(tp->Thread_Shutdown)
	{
 	//添加任务到线程池中
	pthread_mutex_lock(&tp->queue_lock);
	while(tp->Cur==tp->Max)//当满了就挂起在NOT_EMPTY
	{
		pthread_cond_wait(&tp->Not_Empty,&tp->queue_lock);
	}
	if(tp->Thread_Shutdown!=1)
	{
		pthread_mutex_unlock(&tp->queue_lock);
		printf("Producer thread shutdown\n");
		return -1;
	}
	//工作区间
	tp->Container[tp->Front].Business_Addr=business.Business_Addr;
	tp->Container[tp->Front].arg=business.arg;
	tp->Front=(tp->Front+1)%tp->Max;
	tp->Cur++;
	//释放锁
	pthread_cond_signal(&tp->Not_Full);
	pthread_mutex_unlock(&tp->queue_lock);
	}
	return 0;
}
