#include"thread_poll.h"

void* Accept_business(void*arg)
{
	struct sockaddr_in clientaddr;
	int serverfd=*(int*)arg;
	int clientfd;
	clientfd=ACCEPT(serverfd,&clientaddr);
	char IP[16];
	bzero(IP,16);
	char response[100];
	bzero(response,100);
	printf("[%s] [%d] Accept Successfully..\n",inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,IP,16),ntohs(clientaddr.sin_port));
	sprintf(response,"Hi,Welcome to TCP_EPOLL_THREAD_SERVER\n");
	SEND(clientfd,response,strlen(response),0);
	int i=0;
	for(i=0;i<_MAXSOCKET;i++)
	{
		pthread_mutex_lock(&global_lock);
		if(clientfd_list[i]==-1)
		{
			clientfd_list[i]=clientfd;
			pthread_mutex_unlock(&global_lock);
			break;
		}
		pthread_mutex_unlock(&global_lock);
	}
	if(i==_MAXSOCKET)
	{
		printf("Accept Max...\n");
		close(clientfd);
	}
	else
	{
		printf("clientfd %d ADD LISTEN\n",clientfd);
		struct epoll_event node;
		node.data.fd=clientfd;
		node.events=EPOLLIN|EPOLLET;
		if((epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&node))==-1)
		{
			printf("add epfd failed\n");
			exit(0);
		}
	}
	return NULL;
}
