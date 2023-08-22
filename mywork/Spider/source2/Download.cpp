#include"Spider.h"

int Download(url_t*url,int webfd,char*request,SSL_t* ssl)
{
	int code;//响应码
	char buf[8192];//接收缓存区
	char response[4096];//响应头缓冲区
	int len;//响应头长度
	char* pos=NULL;//使用strstr获得的/r/n/r/n的地址
	int fd;//写的文件
	bzero(buf,8192);
	bzero(response,4096);
	if(ssl==NULL)
	{
		//发送请求
		if((send(webfd,request,strlen(request),0)==-1))
		{
			perror("send request failed");
			exit(0);
		}
		printf("[Download] [Step 5] [send request successfully]\n");
		//接受数据
		if((len=recv(webfd,buf,sizeof(buf),0))==-1)
		{
			perror("recv call failed");
			exit(0);
		}
		//查找边界
		if((pos=strstr(buf,"\r\n\r\n"))==NULL)
		{
			printf("strstr call failed\n");
			printf("%s\n",buf);
			exit(0);
		}
		//pos-buf+4 就能算出响应头+空行的大小
		//扣出响应头
		snprintf(response,pos-buf+4,"%s",buf);
		printf("[Download] [Step 6] [Get reponse Head successfully]\n%s",response);
		//检测响应码
		if((code=Spider_Get_Code(response))==200)
		{
			//保存数据
			if((fd=open(url->src_name,O_RDWR|O_CREAT,0664))==-1)
			{
				perror("open call failed");
				exit(0);
			}
			//将剩下的内容写入文件
			write(fd,pos+4,len-(pos-buf+4));
			while((len=recv(webfd,buf,sizeof(buf),0))>0)
			{
				//计算的二进制文件 长度不能用strlen计算,因此剩下的长度就是接收的长度len
				write(fd,buf,len);
				bzero(buf,sizeof(buf));
			}
		}
		else
		{
			printf("[Download][Step 7] [Response Code %d,Download failed\n]",code);
			close(webfd);
			return -1;
		}
	}
	else
	{	
		//发送请求
		if((SSL_write(ssl->sslsocket,request,strlen(request))==-1))
		{
			perror("SSL_write request failed");
			free(ssl);
			ssl=NULL;
			exit(0);
		}
		printf("[Download] [Step 5] [SSL_write request successfully]\n");
		//接受数据
		if((len=SSL_read(ssl->sslsocket,buf,sizeof(buf)))==-1)
		{
			perror("SSL_read call failed");
			free(ssl);
			ssl=NULL;
			exit(0);
		}
		//查找边界
		if((pos=strstr(buf,"\r\n\r\n"))==NULL)
		{
			printf("strstr call failed\n");
			free(ssl);
			ssl=NULL;
			exit(0);
		}
		//pos-buf+4 就能算出响应头+空行的大小
		//扣出响应头
		snprintf(response,pos-buf+4,"%s",buf);
		printf("[Download] [Step 6] [Get HTTPS reponse Head successfully]\n%s",response);
		//检测响应码
		if((code=Spider_Get_Code(response))==200)
		{

			//保存数据
			if((fd=open(url->src_name,O_RDWR|O_CREAT,0664))==-1)
			{
				perror("open call failed");
				free(ssl);
				ssl=NULL;
				exit(0);
			}
			//将剩下的内容写入文件
			write(fd,pos+4,len-(pos-buf+4));
			while((len=SSL_read(ssl->sslsocket,buf,sizeof(buf)))>0)
			{
				//计算的二进制文件 长度不能用strlen计算,因此剩下的长度就是接收的长度len
				write(fd,buf,len);
				bzero(buf,sizeof(buf));
			}
		}
		else
		{
			printf("[Download][Step 7] [Response Code %d,Download failed]\n",code);
			free(ssl);
			ssl=NULL;
			close(webfd);
			return -1;
		}
	}
	printf("[Download] [Step 7][Download successfully]\n");
	free(ssl);
	ssl=NULL;
	close(fd);
	close(webfd);
	return 0;
}
