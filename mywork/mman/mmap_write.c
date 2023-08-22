#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<sys/mman.h>
#include<stdio.h>
#include<string.h>
typedef struct
{
	int id;
	char name[1024];
}shared_t;


int main(void)
{
	int fd;
	fd=open("MMAP_FILE",O_RDWR);
	shared_t *ptr=NULL;
	ftruncate(fd,sizeof(shared_t));
	if((ptr=(shared_t*)mmap(NULL,sizeof(shared_t),PROT_READ|PROT_EXEC|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
	{
		perror("mmap call failed");
		exit(0);
	}
	close(fd);
	ptr->id=0;
	bzero(ptr->name,sizeof(ptr->name));
	while(1)
	{
		(ptr->id)++;
		sprintf(ptr->name,"UserName(%d)",ptr->id);
		sleep(1);
	}
	munmap(ptr,sizeof(shared_t));
	return 0;

}
