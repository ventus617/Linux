#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//子进程分别用创建组和创建会话的方式，脱离会话的限制，避免被杀死
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
		printf("parent PPID %d,pid %d,PGID %d,SID %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
		while(1)
			sleep(1);
	}
	else if(pid==0)
	{
		if(i==0)//创建会话的方式
		{
			printf("child ppid %d,pid %d,pgid %d,sid %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
			sleep(5);
			setsid();//创建组并创建会话
			printf("child ppid %d,pid %d,pgid %d,sid %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
		}
		else if(i==1)//创建组的方式
		{
			
			printf("child ppid %d,pid %d,pgid %d,sid %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
			sleep(5);
			setpgid(getpid(),getpid());
			printf("child ppid %d,pid %d,pgid %d,sid %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
		}
		else
		{	
			printf("child ppid %d,pid %d,pgid %d,sid %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
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
