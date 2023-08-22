#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>

void sig_do(int n)
{
	printf("hello world\n");	
}

unsigned int mysleep(unsigned int seconds)
{
	unsigned int time;
	//设在信号屏蔽-屏蔽信号SIGALRM
	sigset_t set,oset;
	sigemptyset(&set);
	sigaddset(&set,SIGALRM);
	sigprocmask(SIG_SETMASK,&set,&oset);
	//完成信号捕捉
	struct sigaction act,oact;
	act.sa_handler=sig_do;//捕捉函数
	act.sa_flags=0;//捕捉标志
	sigemptyset(&act.sa_mask);//设置临时屏蔽字为空
	sigaction(SIGALRM,&act,&oact);//设置捕捉信号
	time=alarm(seconds);
	sleep(3);
	//sigsuspend临时解除屏蔽+挂起进程
	sigsuspend(&act.sa_mask);//不能用oset 但是第一次可以，第二次不行了，因为调用过sigsuspend函数后oset会变成1,因为又重新进行了一次信号屏蔽，使用1传回到oset了
	//pause();
	return time;
}

int main(void)
{
	while(1)
	{
		printf("2 seconds..\n");
		mysleep(2);
	}
	return 0;
}
