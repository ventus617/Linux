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
	fd=open("namefifo",O_WRONLY);
	printf("阻塞\n");
	write(fd,MSG,strlen(MSG));
	printf("write successfully,pid %d\n",getpid());
	close(fd);
	return 0;
}
