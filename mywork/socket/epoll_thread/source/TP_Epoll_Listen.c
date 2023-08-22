#include"thread_poll.h"

int TP_Epoll_Listen(int sockfd,tp_t*tp)
{
	int readycode=0;
	struct epoll_event nready[_MAXSOCKET];
	int flag=0;
	while(tp->Thread_Shutdown)
	{
		flag=0;
		readycode=epoll_wait(epfd,nready,_MAXSOCKET,0);
		if(readycode==-1)
		{
			perror("TP_Epoll_Listen,Epoll_wait failed");
			exit(0);
		}
		while(readycode)
		{
			if(nready[flag].data.fd==sockfd)
			{
				bs_t bs;
				bs.Business_Addr=Accept_business;
				bs.arg=(void*)&sockfd;
				TP_Producer(tp,bs);
				printf("Add Accept Business...\n");
			}
			else
			{
				printf("nready[flag].fd=%d\n",nready[flag].data.fd);
				bs_t bs;
				bs.Business_Addr=Forward_business;
				bs.arg=(void*)&nready[flag].data.fd;
				TP_Producer(tp,bs);
				printf("Add Forward Business...\n");
			}
			readycode--;
			flag++;
		}
	}
	return 0;
}
