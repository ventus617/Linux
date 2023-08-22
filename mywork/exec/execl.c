#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>


int main(void)
{
	pid_t pid;
	pid=fork();
	if(pid>0)
	{
		printf("parent pid %d running...\n",getpid());
		while(1)
			sleep(1);
	}
	else if(pid==0)
	{
		printf("child pid %d running..\n",getpid());
		execl("/usr/bin/firefox","firefox","https://www.baidu.com",NULL);
	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
	return 0;
}
