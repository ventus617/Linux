#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<time.h>
void Business(void)
{
	char time_buffer[1024];
	bzero(time_buffer,sizeof(time_buffer));
	time_t tp;
	int timeout=5;
	int fd;
	fd=open("time.log",O_RDWR|O_CREAT,0664);
	while(1)
	{
		tp=time(NULL);//获取时间
		ctime_r(&tp,time_buffer);//通过ctime_r函数计算年月日
		write(fd,time_buffer,strlen(time_buffer));
		bzero(time_buffer,sizeof(time_buffer));
		sleep(timeout);
	}
	close(fd);
}

int Daemon_Create(void)
{
	pid_t pid;
	pid=fork();
	if(pid>0)
	{
		sleep(2);
		exit(0);
	}
	else if(pid==0)
	{
		setsid();
		//关闭无用的描述符
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		//改变工作目录
		chdir("./");
		//修改UMASK掩码 0002
		//0002进行位运算取反求与0666 得0664，避免权限异常
		umask(0002);
		//工作内容
		Business();
		//释放资源
	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
}
int main(void)
{
	Daemon_Create();
	return 0;
}
