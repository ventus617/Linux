#include"thread_poll.h"

void* Forward_business(void*arg)
{
	int clientfd=*(int*)arg;
	char buf[1500];
	char Response[1600];
	bzero(buf,1500);
	bzero(Response,1600);
	ssize_t len;
	while((len=RECV(clientfd,buf,sizeof(buf),MSG_DONTWAIT))>0||len==-2)
	{
		if(len==-2)
		{
			continue;
		}
		else
		{
			sprintf(Response,"[TALK]:%s",buf);
			for(int i=0;i<_MAXSOCKET;i++)
			{
			pthread_mutex_lock(&global_lock);	
				if(clientfd_list[i]!=-1)
				{
					SEND(clientfd_list[i],Response,strlen(Response),0);
				}
			pthread_mutex_unlock(&global_lock);
			}
			break;
		}
	}
	if(len==0||len==-1)
	{
		printf("clientfd:%d exiting...\n",clientfd);
		for(int i=0;i<_MAXSOCKET;i++)
		{
			pthread_mutex_lock(&global_lock);
			if(clientfd_list[i]==clientfd)
			{
				clientfd_list[i]=-1;
				epoll_ctl(epfd,EPOLL_CTL_DEL,clientfd,NULL);
				close(clientfd);
				pthread_mutex_unlock(&global_lock);
				break;
			}
			pthread_mutex_unlock(&global_lock);
		}
	}
	return NULL;
}
