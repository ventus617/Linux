#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

//kill(pid,signo);

int main(int argv,char** argc)
{
	if(argv<3)
	{
		printf("Usage error;\n");
		exit(0);
	}
	kill(atoi(argc[1]),atoi(argc[2]));
	return 0;
}
