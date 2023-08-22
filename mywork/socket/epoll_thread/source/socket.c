#include"socket.h"

int SOCKET(int domain,int type,int protocol)
{
	int sockfd=socket(domain,type,protocol);
	if(sockfd==-1)
	{
		perror("socket create failed");
		return -1;
	}
	return sockfd;
}


int BIND(int sockfd,struct sockaddr_in addr)
{
	int err;
	socklen_t addrlen=sizeof(addr);
	if((err=bind(sockfd,(const struct sockaddr*)&addr,addrlen))!=0)
	{
		printf("bind call failed %s\n",strerror(err));
		return -1;
	}
	return 0;
}

int LISTEN(int sockfd,int backlog)
{
	int err;
	if((err=listen(sockfd,backlog))!=0)
	{
		printf("listen call failed %s\n",strerror(err));
		return -1;
	}
	return 0;
}

int CONNECT(int sockfd,struct sockaddr_in addr)
{
	socklen_t addrlen=sizeof(addr);
	int err;
	if((err=connect(sockfd,(const struct sockaddr*)&addr,addrlen))!=0)
	{
		printf("connect call failed %s\n",strerror(err));
		return -1;
	}
	return 0;
}

int ACCEPT(int sockfd,struct sockaddr_in *addr)
{
	struct sockaddr_in clientaddr=*addr;
	socklen_t addrlen=sizeof(clientaddr);
	int clientfd;
	if((clientfd=accept(sockfd,(struct sockaddr*)addr,&addrlen))==-1)
	{
		printf("accept call failed%s \n",strerror(errno));
		return -1;
	}
	return clientfd;
}

ssize_t SEND(int sockfd,const void* buf,size_t len,int flags)
{
	ssize_t sendnum;
	if((sendnum=send(sockfd,buf,len,flags))==-1)
	{
		perror("send call failed");
		exit(0);
	}
	return sendnum;
}

ssize_t RECV(int sockfd,void*buf,size_t len,int flags)
{
	ssize_t recvnum;
	if((recvnum=recv(sockfd,buf,len,flags))==-1)
	{
		if(errno==EAGAIN)
		{
			return -2;
		}
		perror("recv call failed");
		return -1;
	}
	return recvnum;
}
