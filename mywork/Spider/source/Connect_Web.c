#include"Spider.h"

int Connect_Web(url_t* url,int sockfd)
{
	struct sockaddr_in webAddr;
	bzero(&webAddr,sizeof(webAddr));
	webAddr.sin_family=AF_INET;
	webAddr.sin_port=htons(url->port);
	inet_pton(AF_INET,url->ip,&webAddr.sin_addr.s_addr);
	if((connect(sockfd,(struct sockaddr*)&webAddr,sizeof(webAddr)))==-1)
	{
		perror("connect call failed");
		exit(0);
	}
	printf("[Download] [Step 3] [Connection web server successfully]\n");
	return 0;
}
