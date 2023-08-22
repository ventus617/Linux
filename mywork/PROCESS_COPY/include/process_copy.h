#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>


int block_cur(const char*src,int pronum);//根据源文件,返回每个进程负责的大小

//int copy(const char* src,const char* des,copysize,pos);//拷贝函数,根据参赛打开文件描述符,按照固定大小和偏移的位置来拷贝,因此只能是进程重载,不能是函数重载


int check_arg(int argc,const char* src,int prono);//参数校验,进程数量校验和目的文件校验

int process_create(const char* src,const char* des,int prono,int blocksize);//创建进程,excel重新加载copy函数来实现多进程拷贝,prono代表需要偏移的单位个数

int process_wait(void);//做完任务的僵尸进程回收
