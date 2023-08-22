#include"thread_poll.h"

int TP_Destroy(tp_t*tp,int sockfd)
{
	tp->Thread_Shutdown=0;
	free(tp->Container);
	free(tp->cus_tids);
	tp->Container=NULL;
	tp->cus_tids=NULL;
	pthread_mutex_destroy(&tp->queue_lock);
	pthread_mutex_destroy(&tp->var_lock);
	pthread_cond_destroy(&tp->Not_Empty);
	pthread_cond_destroy(&tp->Not_Full);
	pthread_attr_destroy(&thread_attr);
	free(tp);
	tp=NULL;
	close(sockfd);
	return 0;
}
