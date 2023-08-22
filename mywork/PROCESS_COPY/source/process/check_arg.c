#include"process_copy.h"

int check_arg(int argc,const char*src,int prono)
{
	if(argc<2)
	{
		printf("Usage: exename  srcname  processnum\n");
		exit(0);
	}
	if(prono<1||prono>=100)
	{
		printf("error,processnum wrong\n");
		exit(0);
	}
	if(access(src,F_OK)!=0)
	{
		printf("src error,not found\n");
		exit(0);
	}
	return 0;
}
