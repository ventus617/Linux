#include"process_copy.h"

int block_cur(const char* src,int pronum)
{
	int fd;
	if((fd=open(src,O_RDONLY))==-1)
	{
		perror("open src failed");
		exit(0);
	}
	int len=lseek(fd,0,SEEK_END);
	close(fd);
	if(len%pronum==0)
	{
		return len/pronum;
	}
	else
	{
		return len/pronum-1;
	}
}
