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
		execlp("firefox","firefox","https://www.baidu.com",NULL);//前面不用写路径,指的是去查找位置
	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
	return 0;
}
