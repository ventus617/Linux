
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
 
 void sig_do(int n)
 {
 	printf("已经捕捉到信号signal[%d]..\n",n);
 }

 int main(void)
 {
 	struct sigaction act,oact;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	act.sa_handler=sig_do;
	sigaction(SIGINT,&act,&oact);
	while(1)
	{
		sleep(1);
	}
	return 0;
 }
