#include<stdlib.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<ctype.h>
#define _PORT (8080)
#define _IP ("10.0.0.129")
#define _BACKLOG 128
int main(void)
{
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
	//完善并发链接和并发处理
	char request[1500];
	bzero(request,1500);
	int flags=0;//转换的字符数量
	ssize_t len;//读取的数据长度
	int clientfd;
	char client_ip[16];
	char response[1500];
	while(1)
	{
		//接受链接
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
			bzero(response,1500);
			printf("server output:Accept success,client ip %s client port%d\n",inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,client_ip,16),ntohs(clientaddr.sin_port));
			sprintf(response,"H1,%s Welcome to Tcp Server Demo.\n",client_ip);
			send(clientfd,response,strlen(response),0);
			//数据交互
			while((len=recv(clientfd,request,sizeof(request),0))>0)
			{
				flags=0;
				//小写转大写
				while(len>flags)
				{
					request[flags]=toupper(request[flags]);
					flags++;
				}
				send(clientfd,request,strlen(request),0);
				bzero(request,1500);
			}
		}
	}
	close(clientfd);
	printf("server service done...\n");
	close(sockfd);
	return 0;
}
