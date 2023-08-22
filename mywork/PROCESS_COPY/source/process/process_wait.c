#include"process_copy.h"



int process_wait(void)
{
	//非阻塞回收
	int status;
	pid_t zpid;
	while((zpid=waitpid(-1,&status,WNOHANG))!=-1)
	{
		if(zpid==0)continue;
		if(zpid>0)
			printf("Parent process %d,wait zombie %d,status num %d\n",getpid(),zpid,status);
	}
	return 0;
}
