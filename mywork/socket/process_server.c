#include"socket.h"

#define _BACKLOG 128
#define _PORT 8090
#define _IP "10.0.0.129"
//捕捉信号回收处理
void sig_do (int n)
{
	pid_t pid;
	int status;
	while((pid=waitpid(-1,&status,WNOHANG))!=-1)
	{
		if(pid>0)
		{
			printf("recycle child pid%d...\n",pid);
			if(WIFEXITED(status))
			{
				printf("child pid%d exit normally\n",pid);
			}
			else if(WIFSIGNALED(status))
			{
				printf("child pid%d killed by signal\n",pid);
			}
		}
		else if(pid==0)
		{
			break;
		}
	}
}
//普通线程来回收处理
void * thread_wait(void*arg)
{
	pthread_detach(pthread_self());
	//捕捉
	struct sigaction act,oact;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	act.sa_handler=sig_do;
	sigaction(SIGCHLD,&act,&oact);
	//清空屏蔽
	printf("wait thread 0x%x working..\n",(unsigned int)pthread_self());
	sigprocmask(SIG_SETMASK,&act.sa_mask,NULL);
	while(1)
		usleep(100000);
	pthread_exit(NULL);
}
int main(void)
{
	//设置信号屏蔽，创建副线程来回收
	pthread_t tid;
	sigset_t set,oset;
	sigemptyset(&set);
	sigaddset(&set,SIGCHLD);
	sigprocmask(SIG_SETMASK,&set,&oset);
	int err;
	if((err=pthread_create(&tid,NULL,thread_wait,NULL))!=0)
	{
		printf("pthread create call failed %s\n",strerror(err));
		exit(0);
	}
	struct sockaddr_in seraddr;
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(_PORT);
	inet_pton(AF_INET,_IP,&seraddr.sin_addr.s_addr);
	//创建套接字
	int serfd=SOCKET(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//分配ip地址
	BIND(serfd,seraddr);
	//监听套接字
	LISTEN(serfd,_BACKLOG);
	//监听后分配给孩子
	while(1)
	{
		int clientfd;
		struct sockaddr_in clientaddr;
		if((clientfd=ACCEPT(serfd,&clientaddr))!=-1)
		{
			pid_t pid;
			pid=fork();
			//父进程负责打印accept的工作
			if(pid>0)
			{	
				char client_ip[16];
				unsigned short client_port;
				bzero(client_ip,16);
				inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,client_ip,16);
				client_port=ntohs(clientaddr.sin_port);
				printf("IP:[%s] PORT:[%d] ACCEPTED....\n ",client_ip,client_port);
				char request[1000];
				bzero(request,1000);
				sprintf(request,"hi,[%s] Welcome to Process_Server\n",client_ip);
				SEND(clientfd,request,strlen(request),0);
			}
			//子进程负责业务
			else if(pid==0)
			{
				char buf[1500];
				bzero(buf,1500);

				int flags=0;
				ssize_t recvnum;
				while((recvnum=RECV(clientfd,buf,sizeof(buf),0))>0)
				{
					while(flags<recvnum)
					{
						buf[flags]=toupper(buf[flags]);
						flags++;
					}
					SEND(clientfd,buf,strlen(buf),0);
					bzero(buf,1500);
				}
				if(recvnum<=0)
				{
				close(clientfd);
				exit(0);
				}
			}
			else
			{
				perror("fork call failed");
				exit(0);
			}
		}
	}
	close(serfd);
	return 0;
}
