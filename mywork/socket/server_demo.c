#include<stdlib.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#define _PORT (8080)
#define _IP ("10.0.0.129")
#define _BACKLOG 128
int main(void)
{
	pid_t pid;
	pid=fork();
	if(pid==-1)
	{
		perror("fork call failed");
		exit(0);
	}
	if(pid>0)
	{
		while(1)
			sleep(1);
	}
	else if(pid==0)
	{
		//setsid();
		printf("ppid%d pid%d gpid%d sid%d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
		//创建套接字
		struct sockaddr_in serveraddr;
		bzero(&serveraddr,sizeof(serveraddr));
		serveraddr.sin_family=AF_INET;
		serveraddr.sin_port=htons(_PORT);
		//serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
		inet_pton(AF_INET,_IP,&serveraddr.sin_addr.s_addr);
		int sockfd;
		if((sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
		{
			perror("socket call failed");
			exit(0);
		}
		//分配网络地址
		if((bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)))==-1)
		{
			perror("bind call failed");
			exit(0);
		}
		//监听套接字
		if((listen(sockfd,_BACKLOG))==-1)
		{
			perror("listen call failed");
			exit(0);
		}
		//接受链接
		int clientfd;
		struct sockaddr_in clientaddr;
		bzero(&clientaddr,sizeof(clientaddr));
		socklen_t len;
		len=sizeof(clientaddr);
		if((clientfd=accept(sockfd,(struct sockaddr*)&clientaddr,&len))==-1)
		{
			perror("accept call failed");
			exit(0);
		}
		else
		{
			char client_ip[16];
			printf("server output:Accept success,client ip %s client port%d\n",inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,client_ip,16),ntohs(clientaddr.sin_port));
		}
		printf("server service done...\n");
		close(clientfd);
		close(sockfd);
	}
	return 0;
}
