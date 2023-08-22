#include"socket.h"
#define _BACKLOG 128
#define _IP "10.0.0.129"
#define _PORT 8080

pthread_mutex_t lock;
int client_list[1000];

//线程参数
typedef struct
{
	int client_fd;
	char client_ip[16];
}thread_arg;


//转发
void* thread_forward(void*arg)
{
	printf("pthread 0x%x is working\n",(unsigned int )pthread_self());
	pthread_detach(pthread_self());
	thread_arg p_arg=*(thread_arg*)arg;
	ssize_t recvnum;
	char buf[1500];
	char forward[1800];
	bzero(forward,1800);
	bzero(buf,1500);
	int flags=0;
	while((recvnum=RECV(p_arg.client_fd,buf,sizeof(buf),0))>0)
	{
		sprintf(forward,"[%s]:%s\n",p_arg.client_ip,buf);
		printf("%s",forward);
		for(int i=0;i<1000;i++)
		{
			pthread_mutex_lock(&lock);
			if(client_list[i]!=-1)
			{
				SEND(client_list[i],forward,strlen(forward),0);
			}
			pthread_mutex_unlock(&lock);
		}
		bzero(forward,sizeof(forward));
		bzero(buf,sizeof(buf));
	}
	if(recvnum<=0)
	{
		printf("disconnect from client\n");
		for(int i=0;i<1000;i++)
		{
			pthread_mutex_lock(&lock);
			if(client_list[i]==p_arg.client_fd)
			{
				client_list[i]=-1;
				pthread_mutex_unlock(&lock);
				break;
			}
			pthread_mutex_unlock(&lock);
		}
		close(p_arg.client_fd);
	}
	pthread_exit((void*)666);
}

int main(void)
{
	pthread_mutex_init(&lock,NULL);
	for(int i=0;i<1000;i++)
	{
		client_list[i]=-1;
	}
	int server_fd,client_fd;
	struct sockaddr_in serveraddr,clientaddr;
	char response[1500];
	pthread_t tid;
	bzero(response,1500);
	bzero(&serveraddr,sizeof(serveraddr));
	bzero(&clientaddr,sizeof(clientaddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(_PORT);
	inet_pton(AF_INET,_IP,&serveraddr.sin_addr.s_addr);
	server_fd=SOCKET(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	BIND(server_fd,serveraddr);
	LISTEN(server_fd,_BACKLOG);
	//线程参数
	thread_arg arg;
	bzero(arg.client_ip,16);
	//开始准备监听
	while(1)
	{
		client_fd=ACCEPT(server_fd,&clientaddr);
		//记录套接字
		for(int i=0;i<1000;i++)
		{
			pthread_mutex_lock(&lock);
			if(client_list[i]==-1)
			{
				client_list[i]=client_fd;
				pthread_mutex_unlock(&lock);
				break;
			}
			pthread_mutex_unlock(&lock);
		}
		arg.client_fd=client_fd;
		inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,arg.client_ip,16);
		printf("Server [%s] accepted,Clinet Ip [%s] Port[%d]\n",_IP,arg.client_ip,ntohs(clientaddr.sin_port));
		sprintf(response,"hi,%s Welcome  to Tcp Pthread  Server\n",arg.client_ip);
		SEND(client_fd,response,strlen(response),0);
		bzero(response,1500);
		//创建线程
		pthread_create(&tid,NULL,thread_forward,(void*)&arg);
	}
	pthread_mutex_destroy(&lock);
	for(int i=0;i<1000;i++)
	{
		if(client_list[i]!=-1)
		{
			close(client_list[i]);
			client_list[i]=-1;
		}
	}
	return 0;
}
