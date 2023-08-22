#include"thread_poll.h"

int TP_Netinit(void)
{
	//初始化全局资源
	pthread_mutex_init(&global_lock,NULL);
	for(int i=0;i<_MAXSOCKET;i++)
	{
		clientfd_list[i]=-1;
	}
	//创建套接字
	//分配IP地址
	//监听LISTEN
	int serverfd;
	serverfd=SOCKET(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	struct sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(_PORT);
	inet_pton(AF_INET,_IP,&serveraddr.sin_addr.s_addr);
	if((BIND(serverfd,serveraddr)==-1))
	{
		exit(0);
	}
	LISTEN(serverfd,_BACKLOG);
	return serverfd;
}
