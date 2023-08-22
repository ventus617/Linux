#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<sys/types.h>

//观测到 两个文件去设置文件锁，会阻塞等待另一个写完
int main(void)
{
	int fd=open("file",O_RDWR|O_CREAT,0664);
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	//阻塞设置文件锁
	fcntl(fd,F_SETLKW,&lock);
	printf("process a %d,set wrlock successfully\n",getpid());
	sleep(20);
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLKW,&lock);
	printf("process a %d,set unlock successfully\n",getpid());
	close(fd);
	return 0;

}
