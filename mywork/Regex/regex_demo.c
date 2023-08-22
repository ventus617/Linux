//<a href="https://www.toutiao.com/video/7185502712392319521/" target="_blank" rel="noopener nofollow" class="title">S30新赛季最强打野：这8位制霸野区，谁用谁赢！</a>

#include<stdio.h>
#include<stdlib.h>
#include<regex.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main(void)
{
	const char* parr_str="<a href=\"\\(https://www\\.toutiao\\.com/video/[^\"]\\+\\?\\)\"[^>]\\+\\?>\\([^<]\\+\\?\\)</a>";
	//需要两个转译字符 因为C解释器需要解释一个转译字符 正则解释器也需要一个
	regex_t reg;
	regmatch_t ma[3];
	regcomp(&reg,parr_str,0);
	//数据映射
	int fd=open("今日头条.html",O_RDONLY);
	int size=lseek(fd,0,SEEK_END);
	char*fd_str=NULL;
	char*fd_str_copy=NULL;
	//需要一遍匹配一遍偏移的
	if((fd_str=(char*)mmap(NULL,size,PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED)
	{
		perror("mmap call failed");
		exit(0);
	}
	fd_str_copy=fd_str;
	char buf[1000];
	bzero(buf,1000);
	int errsize;
	int error;
	char link[4096];
	char title[4096];
	bzero(link,sizeof(link));
	bzero(title,sizeof(title));
	while((error=regexec(&reg,fd_str,3,ma,0))!=REG_NOMATCH)
	{
		//通过位置提取数据
		snprintf(title,ma[2].rm_eo-ma[2].rm_so+1,"%s",fd_str+ma[2].rm_so);//大小是rm_eo-rm_so+1,起始位置是fd_str+rm_so
		snprintf(link,ma[1].rm_eo-ma[1].rm_so+1,"%s",fd_str+ma[1].rm_so);
		printf("%s\n %s\n",link,title);
		bzero(title,sizeof(title));
		bzero(link,sizeof(link));
		//偏移地址
		fd_str+=ma[0].rm_eo;
	}
	errsize=regerror(error,&reg,buf,sizeof(buf));
	printf("error:%d errsize:%d buf:%s\n",error,errsize,buf);
	regfree(&reg);
	close(fd);
	munmap(fd_str_copy,size);
	return 0;
}
