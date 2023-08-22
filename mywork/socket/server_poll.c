#include"socket.h"
#include<poll.h>

#define _IP "10.0.0.129"
#define _PORT 8080
#define _BACKLOG 128
int main(void)
{
	struct sockaddr_in serveraddr,clientaddr;
	int serverfd;
	struct pollfd clientfd[1020];
	for(int i=0;i<1020;i++)
	{
		clientfd[i].fd=-1;
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
	//先把服务器socket添加到set中
	clientfd[0].fd=serverfd;
	clientfd[0].events=POLLIN;//监听读事件
	int readycode=0;
	while(1)
	{
		readycode=poll(clientfd,1020,-1);
		while(readycode)
		{
			//发现是对serverfd的套接字请求
			if(clientfd[0].revents==POLLIN)
			{
				int tmp_fd=ACCEPT(serverfd,&clientaddr);
				//找到能存的位置，赋值
				for(int i=1;i<1020;i++)
				{
					if(clientfd[i].fd==-1)
					{
						clientfd[i].fd=tmp_fd;
						clientfd[i].events=POLLIN;//设置监听事件
						break;	
					}
				}
				char response[1500];
				bzero(response,1500);
				char client_ip[16];
				bzero(client_ip,16);
				printf("[%s] Accepted,[%d]port\n",inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,client_ip,16),ntohs(clientaddr.sin_port));
				sprintf(response,"hi,%s Welcome to TCP_IO DEMO",client_ip);
				SEND(tmp_fd,response,strlen(response),0);
				//POLL处理完一次后，也得把revents初始化一次
				clientfd[0].revents=0;
			}
			else
			{
				//在for循环里面找对应的clientfd
				for(int i=1;i<1020;i++)
				{
					if(clientfd[i].fd!=-1)
					{
						if(clientfd[i].revents==POLLIN)
						{
							//不允许while循环读,触发一次只能读一次(因为是单进程服务器)
							recvnum=RECV(clientfd[i].fd,buf,sizeof(buf),0);
							if(recvnum>0)
							{
								sprintf(forward,"[TALK]:%s",buf);
								printf("%s\n",forward);
								for(int j=1;j<1020;j++)
								{
									if(clientfd[j].fd!=-1)
									{
										SEND(clientfd[j].fd,forward,strlen(forward),0);
									}
								}
								bzero(forward,sizeof(forward));
								bzero(buf,sizeof(buf));
							}
							//如果断开链接,回收对应的资源
							if(recvnum==0)
							{
								printf("clientfd:%d  exit...\n",clientfd[i].fd);
								clientfd[i].fd=-1;//取消监听就置成-1
								close(clientfd[i].fd);
							}
							//处理完之后把对应的传出事件赋为空,不会再次进入同一个套接字
							clientfd[i].revents=0;
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
