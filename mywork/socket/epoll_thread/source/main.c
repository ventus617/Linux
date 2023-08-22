#include"thread_poll.h"
int serverfd;
tp_t* tp;
void sig_do(int n)
{
	TP_Destroy(tp,serverfd);
	exit(0);
}
int main(int argc,char**argv)
{
	int TMax;
	int TMin;
	int CMax;
	if(argc!=4)
	{
		printf("Usage: TMax TMin CMax\n");
		exit(0);
	}
	else
	{
		TMax=atoi(argv[1]);
		TMin=atoi(argv[2]);
		CMax=atoi(argv[3]);
	}
	struct sigaction act,oact;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	act.sa_handler=sig_do;
	sigaction(SIGINT,&act,&oact);
	serverfd=TP_Netinit();
	TP_Epoll_Create(serverfd);
	tp=TP_Create(TMax,TMin,CMax);
	TP_Epoll_Listen(serverfd,tp);
	return 0;
}
