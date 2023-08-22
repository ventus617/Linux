#include"socket.h"

#define _PORT 8080
#define _IP "10.0.0.129"

int main(void)
{
	struct sockaddr_in destaddr;
	bzero(&destaddr,sizeof(destaddr));
	destaddr.sin_family=AF_INET;
	destaddr.sin_port=htons(_PORT);
	inet_pton(AF_INET,_IP,&destaddr.sin_addr.s_addr);
	char buf[1500];
	char response[1500];
	bzero(buf,1500);
	bzero(response,1500);
	//创建套接字
	int client_fd=SOCKET(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	CONNECT(client_fd,destaddr);
	RECV(client_fd,buf,sizeof(buf),0);
	printf("%s\n",buf);
	bzero(buf,sizeof(buf));

	//设置非阻塞的STDIN_FILENO
	int flags;
	flags=fcntl(STDIN_FILENO,F_GETFL,0);
	flags|=O_NONBLOCK;
	fcntl(STDIN_FILENO,F_SETFL,flags);
	int len;
	int recvnum;
again:
	//开始读取数据
	while((len=read(STDIN_FILENO,response,sizeof(response)))>0)
	{
		SEND(client_fd,response,strlen(response),0);
		//非阻塞接受数据
		if((recvnum=RECV(client_fd,buf,sizeof(buf),MSG_DONTWAIT))==-1)
		{
			if(errno==EAGAIN)
			{
				bzero(response,sizeof(response));
				bzero(buf,sizeof(buf));
				continue;
			}
			else
			{
				perror("recv call failed");
			}
		}
		printf("%s",buf);
		bzero(buf,sizeof(buf));
		bzero(response,sizeof(response));
	}
	//非阻塞的读取返回-1
	if(len==-1)
	{
		if(errno==EAGAIN)
		{
			//继续重新读数据
			//非阻塞接受数据
			if((recvnum=RECV(client_fd,buf,sizeof(buf),MSG_DONTWAIT))==-1)
			{
				if(errno==EAGAIN)
				{
					bzero(response,sizeof(response));
					bzero(buf,sizeof(buf));
				}
				else
				{
					perror("recv call failed");
				}
			}
			if(recvnum==0)
			{
				printf("connection disconnect..\n");
				close(client_fd);
				exit(0);
			}
			printf("%s",buf);
			bzero(buf,sizeof(buf));
			goto again;
		}
	}
	close(client_fd);
	return 0;
}	
