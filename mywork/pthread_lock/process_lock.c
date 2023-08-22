#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<pthread.h>
#include<sys/mman.h>
//进程间共享访问
typedef struct
{
	int var;
	pthread_mutex_t lock;
}shared_t;

//使得进程能够使用共享的数据，就用mmap 全局也不行
int main(void)
{
	//创建映射文件
	int fd=open("PROCESS_LOCK",O_RDWR|O_CREAT,0664);
	ftruncate(fd,sizeof(shared_t));//拓展空文件,内容大小为结构体大小的空间,相当于给ptr申请了可以使用的空间
	shared_t*ptr=NULL;
	if((ptr=mmap(NULL,sizeof(shared_t),PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,fd,0))==MAP_FAILED)
	{
		perror("mmap call failed");
		exit(0);
	}
	//初始化共享数据
	ptr->var=0;
	//初始化锁属性
	pthread_mutexattr_t newattr;
	pthread_mutexattr_init(&newattr);
	pthread_mutexattr_setpshared(&newattr,PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&(ptr->lock),&newattr);
	pid_t pid;
	pid=fork();
	if(pid>0)
	{
		//父进程加锁
		for(int i=0;i<10000;i++)
		{
			pthread_mutex_lock(&ptr->lock);
			printf("parent pid %d,var++=%d\n",getpid(),++(ptr->var));
			pthread_mutex_unlock(&ptr->lock);
		}
	}
	else if(pid==0)
	{
		//子进程加锁
		for(int i=0;i<10000;i++)
		{
			pthread_mutex_lock(&ptr->lock);
			printf("child pid %d,var++=%d\n",getpid(),++(ptr->var));
			pthread_mutex_unlock(&ptr->lock);
		}
	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
	//回收资源
	wait(NULL);
	pthread_mutexattr_destroy(&newattr);
	pthread_mutex_destroy(&ptr->lock);
	munmap(ptr,sizeof(shared_t));//最后再释放申请的空间
	return 0;
}
