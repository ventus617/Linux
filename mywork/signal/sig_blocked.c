#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

//屏蔽字

int main(void)
{
	sigset_t set,oset;//信号集类型
	sigemptyset(&set);//初始化信号集,所有位码为0
	sigfillset(&set);//初始化信号集,所有位码为1
	sigaddset(&set,SIGINT);//将set中对应的SIGINT信号的位码设置为1
	sigdelset(&set,SIGINT);//将SET中对应的SGINT的信号的位码设在为0
	//[0][0][1][0]
	int bitcode=sigismember(&set,SIGQUIT);//获取SET信号集里特定信号的位码
	sigprocmask(SIG_SETMASK,&set,&oset);//替换进程的屏蔽字位set，将默认的屏蔽字传出到oset中，SIG_SETMASK是替换的一种方式
	return 0;
}
