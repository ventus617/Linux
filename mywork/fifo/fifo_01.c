#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define MSG "Could U Hear Me?"
//父写子读
int main(void)
{
	pid_t pid;
	int fds[2];
	if(pipe(fds)==-1)
	{
		perror("pipe call failed");
		exit(0);
	}
	pid=fork();
	if(pid>0)
	{
		close(fds[0]);
		printf("Parent %d Send MSG..\n",getpid());
		write(fds[1],MSG,strlen(MSG));
		close(fds[1]);
		wait(NULL);
	}
	else if(pid==0)
	{
		char buf[4096];
		bzero(buf,sizeof(buf));
		close(fds[1]);
		read(fds[0],buf,sizeof(buf));
		printf("Child recv MSG:%s\n",buf);
		close(fds[0]);
		printf("Child Has Done\n");
		exit(0);
	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
	return 0;
}
