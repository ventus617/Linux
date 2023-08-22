
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

//打印信号集
void print_pset(sigset_t pset)
{
	int signo;
	for(signo=1;signo<32;signo++)
	{
		if((sigismember(&pset,signo))==1)
			putchar('1');
		else
			putchar('0');
	}
	putchar('\n');
}

int main(void)
{
	//设置信号集，屏蔽2和3号
	sigset_t set,oset,pset;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	sigaddset(&set,SIGQUIT);
	sigprocmask(SIG_SETMASK,&set,&oset);
	while(1)
	{
		sleep(1);
		sigpending(&pset);//获得当前的未决信号集
		print_pset(pset);//打印信号
	}
	return 0;
}
