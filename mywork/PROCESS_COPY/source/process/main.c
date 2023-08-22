#include"process_copy.h"


int main(int argc,char** argv)
{
	int prono;
	if(argv[3]==0)
	{
		prono=4;
	}
	else
		prono=atoi(argv[3]);
	check_arg(argc,argv[1],prono);
	int blocksize=block_cur(argv[1],prono);
	process_create(argv[1],argv[2],prono,blocksize);
	return 0;
}
