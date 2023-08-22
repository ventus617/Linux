#include"thread_poll.h"

int TP_Epoll_Create(int sockfd)
{
	epfd=epoll_create(_MAXSOCKET);
	//创建EPOLL树
	//添加服务器sockfd到监听树中
	struct epoll_event node;
	node.data.fd=sockfd;
	node.events=EPOLLIN|EPOLLET;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&node);
	return 0;
}
