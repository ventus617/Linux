#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>


#define MSG "Can U Hear Me?"
int main(void)
{
	int fd;
	fd=open("namefifo",O_RDONLY);
	printf("阻塞\n");
	char buf[4096];
	bzero(buf,sizeof(4096));
	read(fd,buf,sizeof(buf));
	printf("read:%s,pid %d\n",buf,getpid());
	close(fd);
	return 0;
}
