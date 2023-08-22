#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<sys/mman.h>

int main(void)
{
	//打开映射文件
	int fd;
	fd=open("MMAP_FILE",O_RDWR);
	int size;
	size=lseek(fd,0,SEEK_END);

	//共享映射,动态维护内存
	int* ptr=NULL;
	if((ptr=mmap(NULL,size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,fd,0))==MAP_FAILED)
	{
		perror("mmap call failed");
		exit(0);
	}
	close(fd);
	//修改映射内存
	//文件是按照字符型存储的,ptr是int类型的,ptr[0]就是从头开始的,ptr[1]代表从第5个字符开始
	ptr[1]=0x34333231;
	//按照16进制修改,用ASCII
	munmap(ptr,size);
	return 0;
}
