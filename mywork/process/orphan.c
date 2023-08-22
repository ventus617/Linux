#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//孤儿进程，就是父进程先走了
int main(void)
{
	pid_t pid;
	pid=fork();
	if(pid>0)
	{
		printf("parent pid%d,ruuning..\n",getpid());
		sleep(1);
		printf("parent pid %d,exit\n",getpid());
		//正常退出
	}
	else if(pid==0)
	{
		//一直等待
		printf("child pid %d ppid%d \n",getpid(),getppid());
		sleep(5);
		printf("child pid %d ppid%d \n",getpid(),getppid());
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
