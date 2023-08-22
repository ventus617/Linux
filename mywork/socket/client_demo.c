#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/fcntl.h>

#define DEST_IP "10.0.0.129"
#define DEST_PORT 8080

int main(void)
{
	char buf[1500];
	bzero(buf,1500);
	int mysock;
	if((mysock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
	{
		perror("socket call failed");
		exit(0);
	}
	struct sockaddr_in destaddr;
	bzero(&destaddr,sizeof(destaddr));
	destaddr.sin_family=AF_INET;
	destaddr.sin_port=htons(DEST_PORT);
	inet_pton(AF_INET,DEST_IP,&destaddr.sin_addr.s_addr);
	if((connect(mysock,(const struct sockaddr*)&destaddr,sizeof(destaddr)))==0)
	{
		recv(mysock,buf,sizeof(buf),0);
		printf("%s\n",buf);
	}
	//信息收发采用非阻塞
	//非阻塞读取标准输入，而后向服务器发送数据
	//STDIN_FILENO,//对文件描述符设置非阻塞
	//int flags;
	//flags=fcntl(STDIN_FILENO,F_GETFL,0);//获取属性传出到flags
	//flags|=O_NONBLOCK;//设在非阻塞
	//fcntl(STDIN_FILENO,F_SETFL,flags);//设置非阻塞
	//非阻塞读取socket，显示在屏幕上
	//recv(mysock,buf,sizeof(buf),MSG_DONTWAIT);
	bzero(buf,sizeof(buf));
	while((fgets(buf,sizeof(buf),stdin))!=NULL)
	{
		send(mysock,buf,strlen(buf),0);
		recv(mysock,buf,sizeof(buf),0);
		printf("response:%s\n",buf);
		bzero(buf,sizeof(buf));
	}
	close(mysock);
	return 0;
}
