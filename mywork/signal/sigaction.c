#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

//捕捉函数,必须要有参数 int n
void sig_do(int n)
{
	int flags=3;
	while(flags)
	{
		printf("\t\t\tflags=%d\n",flags--);
		sleep(1);
	}
}


int main(void)
{
	struct sigaction act,oact;//信号行为结构体
	act.sa_handler=&sig_do;//传函数地址
	//设置信号的行为
	//act.sa_handler=SIG_DFL;//SIG_DFL是默认,SIG_IGN是忽略,传函数地址就是捕捉了
	act.sa_flags=0;//和sa_handler接口对应,默认flags为0
	sigemptyset(&act.sa_mask);//临时屏蔽字 设置临时屏蔽字为空
	sigaction(SIGINT,&act,&oact);//替换行为结构体,将进程原有的换出到oact,替换新的act
	while(1)
		sleep(1);
	return 0;
}
