#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<sys/mman.h>
#include<stdio.h>

typedef struct
{
	int id;
	char name[1024];
}shared_t;


int main(void)
{
	int fd=open("MMAP_FILE",O_RDWR);
	shared_t* ptr=NULL;
	if((ptr=mmap(NULL,sizeof(shared_t),PROT_WRITE|PROT_READ|PROT_EXEC,MAP_SHARED,fd,0))==MAP_FAILED)
	{
		perror("mmap call failed");
		exit(0);
	}
	close(fd);
	while(1)
	{
		printf("id %d,name %s\n",ptr->id,ptr->name);
		sleep(1);
	}
	munmap((void*)ptr,sizeof(shared_t));
	return 0;
}
