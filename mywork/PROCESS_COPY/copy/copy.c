#include"process_copy.h"

// ./copy "src" "desc" "blocksize" "pos"
int main(int argc,char**argv)
{
	int srcfd;
	int destfd;
	int pos;
	int blocksize;
	pos=(atoi(argv[4]));
	blocksize=(atoi(argv[3]));
	char buf[blocksize];
	bzero(buf,sizeof(buf));
	printf("copy child %d,blocksize %d,pos %d\n",getpid(),blocksize,pos);
	//打开文件
	if((srcfd=open(argv[1],O_RDONLY))==-1)
	{
		perror("copy open src failed");
		exit(0);
	}
	if((destfd=open(argv[2],O_RDWR|O_CREAT,0664))==-1)
	{
		perror("copy open dest failed");
		exit(0);
	}
	//以文件头为起点偏移pos
	lseek(srcfd,pos,SEEK_SET);
	lseek(destfd,pos,SEEK_SET);
	int len=read(srcfd,buf,sizeof(buf));
	//写入文件
	write(destfd,buf,len);
	close(srcfd);
	close(destfd);
	return 0;
}
