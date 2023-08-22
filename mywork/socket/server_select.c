#include"socket.h"
#include<sys/select.h>

#define _IP "10.0.0.129"
#define _PORT 8080
#define _BACKLOG 128
int main(void)
{
	struct sockaddr_in serveraddr,clientaddr;
	int serverfd;
	int clientfd[1020];
	for(int i=0;i<1020;i++)
	{
		clientfd[i]=-1;
	}
	char forward[1800];
	bzero(forward,1800);
	char buf[1500];
	bzero(buf,1500);
	ssize_t recvnum;
	//创建套接字
	serverfd=SOCKET(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//分配ip地址
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(_PORT);
	inet_pton(AF_INET,_IP,&serveraddr.sin_addr.s_addr);
	BIND(serverfd,serveraddr);
	//监听集合
	LISTEN(serverfd,_BACKLOG);
	//等待链接
	fd_set set,oset;
	//先把服务器socket添加到set中
	FD_ZERO(&set);
	FD_SET(serverfd,&set);
	int maxfd=serverfd;//记录最大socket
	int readycode=0;
	while(1)
	{
		oset=set;
		readycode=select(maxfd+1,&oset,NULL,NULL,NULL);
		while(readycode)
		{
			//发现是对serverfd的套接字请求
			if(FD_ISSET(serverfd,&oset)==1)
			{
				int tmp_fd=ACCEPT(serverfd,&clientaddr);
				//找到能存的位置，赋值
				for(int i=0;i<1020;i++)
				{
					if(clientfd[i]==-1)
					{
						clientfd[i]=tmp_fd;
						break;	
					}
				}
				char response[1500];
				bzero(response,1500);
				if(tmp_fd>maxfd)maxfd=tmp_fd;
				FD_SET(tmp_fd,&set);
				char client_ip[16];
				bzero(client_ip,16);
				printf("[%s] Accepted,[%d]port\n",inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,client_ip,16),ntohs(clientaddr.sin_port));
				//处理完一次之后就把对应的位码置0,防止再次进入同一个套接字
				sprintf(response,"hi,%s Welcome to TCP_IO DEMO",client_ip);
				SEND(tmp_fd,response,strlen(response),0);
				FD_CLR(serverfd,&oset);
			}
			else
			{
				//在for循环里面找对应的clientfd
				for(int i=0;i<1020;i++)
				{
					if(clientfd[i]!=-1)
					{
						if(FD_ISSET(clientfd[i],&oset)==1)
						{
							//不允许while循环读,触发一次只能读一次(因为是单进程服务器)
							recvnum=RECV(clientfd[i],buf,sizeof(buf),0);
							if(recvnum>0)
							{
								sprintf(forward,"[TALK]:%s",buf);
								printf("%s\n",forward);
								for(int j=0;j<1020;j++)
								{
									if(clientfd[j]!=-1)
									{
										SEND(clientfd[j],forward,strlen(forward),0);
									}
								}
								bzero(forward,sizeof(forward));
								bzero(buf,sizeof(buf));
							}
							//如果断开链接,回收对应的资源
							if(recvnum==0)
							{
								printf("clientfd:%d  exit...\n",clientfd[i]);
								clientfd[i]=-1;
								FD_CLR(clientfd[i],&set);
								close(clientfd[i]);
							}
							//处理完之后把对应位码置0,不会再次进入同一个套接字
							FD_CLR(clientfd[i],&oset);
							break;//本次执行完成,跳出该伦循环
						}
					}
				}
			}
			readycode--;
		}
	}
	close(serverfd);
	return 0;
}
