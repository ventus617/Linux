#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/stat.h>
#include<signal.h>
#include<sys/wait.h>

//int siginfo_t* void* sig_do
//pid_t int union sigval sigqueue
void sig_do(int n,siginfo_t* info ,void* ptr)
{
	int num=info->si_int+1;
	union sigval val;
	val.sival_int=num;
	if(n==SIGUSR1)
	{
		printf("子进程%d 得到消息:%d\n",getpid(),num);
		sigqueue(getppid(),SIGUSR2,val);
	}
	else if(n==SIGUSR2)
	{
		printf("父进程%d 得到消息:%d\n",getpid(),num);
		sigqueue(getpid()+1,SIGUSR1,val);
	}
	sleep(1);
}


int main(int argv,char**argc)
{
	pid_t pid;
	//为了防止把子进程杀死,设置信号屏蔽字
	sigset_t set,oset;
	sigemptyset(&set);
	sigaddset(&set,SIGUSR1);
	sigprocmask(SIG_SETMASK,&set,&oset);
	pid=fork();
	if(pid>0)
	{
		int num=atoi(argc[1]);
		int status;
		pid_t zpid;
		//捕捉信号2
		struct sigaction act,oact;
		sigemptyset(&act.sa_mask);
		act.sa_sigaction=&sig_do;
		act.sa_flags=SA_SIGINFO;
		sigaction(SIGUSR2,&act,&oact);
		//关闭屏蔽
		sigemptyset(&set);
		sigprocmask(SIG_SETMASK,&set,&oset);
		//发送信号1
		union sigval val;
		val.sival_int=num;
		sigqueue(pid,SIGUSR1,val);
		while(1)
		{
			if((zpid=waitpid(-1,&status,WNOHANG)))
			{
				if(WIFEXITED(status))
				{
					printf("回收成功,僵尸进程%d,子进程正常退出,回收码%d\n",zpid,status);
					break;
				}
				else if(WIFSIGNALED(status))
				{
					printf("回收成功,僵尸进程%d,子进程被信号杀死退出,回收码%d\n",zpid,status);
					break;
				}

			}
		}
	}
	else if(pid==0)
	{
		//捕捉函数USR1
		struct sigaction act,oact;
		sigemptyset(&act.sa_mask);
		act.sa_flags=SA_SIGINFO;
		act.sa_sigaction=&sig_do;
		sigaction(SIGUSR1,&act,&oact);
		//解除屏蔽字USR1
		sigset_t set,oset;
		sigemptyset(&set);
		sigprocmask(SIG_SETMASK,&set,&oset);
		while(1);
	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
	return 0;
}
