#include"process_copy.h"



int process_create(const char*src,const char* desc,int prono,int blocksize)
{
	pid_t pid;
	int i;
	for(i=0;i<prono;i++)
	{
		pid=fork();
		if(pid==0)break;
	}
	if(pid>0)
	{
		printf("Parent %d Running..\n",getpid());
		process_wait();
	}
	else  if(pid==0)
	{
		int mypos=blocksize*i;
		char str_pos[10];
		char str_blocksize[10];
		sprintf(str_pos,"%d",mypos);
		sprintf(str_blocksize,"%d",blocksize);
		if(execl("/home/ventus/mywork/PROCESS_COPY/mod/copy","copy",src,desc,str_blocksize,str_pos,NULL)==-1)
		{
			perror("execl failed");
			exit(0);
		}
	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
	return 0;
}
