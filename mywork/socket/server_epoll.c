#include"socket.h"
#include<sys/epoll.h>

#define _IP "10.0.0.129"
#define _PORT 8080
#define _BACKLOG 128
#define _EPOLL_MAX 100000
int main(void)
{
	struct sockaddr_in serveraddr,clientaddr;
	int serverfd;
	int clientfd[_EPOLL_MAX];
	for(int i=0;i<_EPOLL_MAX;i++)
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
	//创建监听树
	//把服务器socket添加到监听树中
	struct epoll_event node;
	node.data.fd=serverfd;
	node.events=EPOLLIN;
	int epfd=epoll_create(_EPOLL_MAX);
	epoll_ctl(epfd,EPOLL_CTL_ADD,serverfd,&node);
	//定义就绪队列
	struct epoll_event nready[_EPOLL_MAX];
	int readycode=0;
	int flag=0;
	while(1)
	{
		flag=0;//因为每次都是传出nready，里面全是就绪的socket,因此可以利用flag,每次循环就+1,就处理一个nready的套接字，就不用再去比较是否是服务器的还是客户端的了,也不用每次去清除一下了
		readycode=epoll_wait(epfd,nready,_EPOLL_MAX,0);
		while(readycode)
		{
			//发现是对serverfd的套接字请求
			if(nready[flag].data.fd==serverfd)
			{
				int tmp_fd=ACCEPT(serverfd,&clientaddr);
				//找到能存的位置，赋值
				for(int i=0;i<_EPOLL_MAX;i++)
				{
					if(clientfd[i]==-1)
					{
						clientfd[i]=tmp_fd;
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
				//添加监听
				node.data.fd=tmp_fd;
				node.events=EPOLLIN;
				epoll_ctl(epfd,EPOLL_CTL_ADD,tmp_fd,&node);
			}
			else
			{
				//不允许while循环读,触发一次只能读一次(因为是单进程服务器)
				recvnum=RECV(nready[flag].data.fd,buf,sizeof(buf),0);
				if(recvnum>0)
				{
					sprintf(forward,"[TALK]:%s",buf);
					printf("%s\n",forward);
					for(int j=0;j<_EPOLL_MAX;j++)
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
					printf("clientfd:%d  exit...\n",nready[flag].data.fd);
					epoll_ctl(epfd,EPOLL_CTL_DEL,nready[flag].data.fd,NULL);
					for(int i=0;i<_EPOLL_MAX;i++)
					{
						if(clientfd[i]==nready[i].data.fd)
						{
							clientfd[i]=-1;
							break;
						}
					}
				}
			}
			++flag;
			readycode--;
		}
	}
	close(serverfd);
	return 0;
}
