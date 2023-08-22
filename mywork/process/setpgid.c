#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<unistd.h>
int main(void)
{
	pid_t pid;
	int i;
	for(i=0;i<3;i++)
	{
		pid=fork();
		if(pid==0)break;
	}
	if(pid>0)
	{
		printf("parent pid %d,PGID %d\n",getpid(),getpgrp());
		while(1)
			sleep(1);
	}
	else if(pid==0)
	{
		if(i==0)
		{
			printf("chile pid %d,pgid %d\n",getpid(),getpgrp());
			sleep(5);
			setpgid(getpid(),getpid());//创建自己的组
			printf("child pid %d,pgid %d\n",getpid(),getpgrp());
		}
		else
		{
			printf("chile pid %d,pgid %d\n",getpid(),getpgrp());
		}
		while(1)
			sleep(1);
	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
	return 0;
}
