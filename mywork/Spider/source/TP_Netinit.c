#include"Spider.h"

int TP_Netinit(void)
{
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(8080);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	int sockfd;
	//创建sock
	if((sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
	{
		perror("socket call failed");
		exit(0);
	}
	printf("[Download] [Step 1] [Net_Init Successfully]\n");
	/* 没必要绑定ip地址,和浏览器没关系
	//分配ip地址
	if((bind(sockfd,(const struct sockaddr*)&addr,sizeof(addr)))!=0)
	{
		perror("bind call failed");
		exit(0);
	}
	*/
	return sockfd;
}

