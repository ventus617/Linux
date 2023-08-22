#include"Spider.h"

int Analyze_html(url_t* url,Container* wct,Container* yct,int& analyze_shutdown,int& result_num,unsigned int* bit_array,int result_fd)
{
    //打开下载的html源码文件,按照mmap共享的方式读取打开
	int fd;
	int filesize;
	char* ptr=NULL;
	char* cp_ptr=NULL;
	char h1[1024];
	char desc[8192];
	char ulink[8192];
	bzero(h1,1024);
	bzero(desc,8192);
	bzero(ulink,8192);
	url_t tmpUrl;
	fd=open(url->src_name,O_RDONLY);
	filesize=lseek(fd,0,SEEK_END);
	if((ptr=(char*)mmap(NULL,filesize,PROT_READ,MAP_PRIVATE,fd,0))==MAP_FAILED)
	{
		perror("Analyze html ,mmap call failed\n");
		exit(0);
	}
	cp_ptr=ptr;
	close(fd);
	//审查元素可能会忽略掉空格
	//<h1 >title</h1>
	//<meta name="description" content="描述数据">
	//<a href="/item/xxxx"></a>
	regex_t hreg,dreg,areg;
	regmatch_t hma[2];
	regmatch_t dma[2];
	regmatch_t ama[2];
	regcomp(&hreg,"<h1 *>\\([^<]\\+\\?\\)</h1>",0);
	regcomp(&dreg,"<meta name=\"description\" content=\"\\([^\"]\\+\\?\\)\"",0);
	regcomp(&areg,"<a[^>]\\+\\?href=\"\\(/item/[^\"]\\+\\?\\)\"[^>]\\+\\?>[^<]\\+\\?</a>",0);
	//开始匹配,获取字符
	if((regexec(&hreg,ptr,2,hma,0))==0)
	{
		snprintf(h1,hma[1].rm_eo-hma[1].rm_so+1,"%s",ptr+hma[1].rm_so);
	}
	if((regexec(&dreg,ptr,2,dma,0))==0)
	{
		snprintf(desc,dma[1].rm_eo-dma[1].rm_so+1,"%s",ptr+dma[1].rm_so);
	}
	++(result_num);
	Save_result(h1,desc,result_fd,result_num);//持久化存储数据
	//开始处理link链接
	if(analyze_shutdown)
	{
		while((regexec(&areg,ptr,2,ama,0)==0))
		{
			snprintf(ulink,ama[1].rm_eo-ama[1].rm_so+24,"https://baike.baidu.com%s",ptr+ama[1].rm_so);
			//去重校验
			if(Remove_Bloom(ulink,strlen(ulink),bit_array)==0)
			{
				//成功就放入未处理队列里面
				strcpy(tmpUrl.origin_url,ulink);
				//放入布隆过滤器
				Insert_Bloom(ulink,strlen(ulink),bit_array);
				Container_set(wct,tmpUrl);
			}
			bzero(ulink,sizeof(ulink));
			ptr+=ama[0].rm_eo;//偏移掉第一个ama的地址
			//如果wct满了就停
			if(wct->cur==wct->max)
			{
				printf("WContainer is Full..\n");
				analyze_shutdown=0;
				break;
			}
		}
	}
	else
	{
		if(wct->cur*100/wct->max<=30)
		{
			printf("Analyze_shutdown start working...\n");
			analyze_shutdown=1;
		}
	}
	unlink(url->src_name);//删除源码,用于删除一个文件或符号链接。 它需要一个参数，即要删除的文件或符号链接的路径名
	regfree(&hreg);
	regfree(&dreg);
	regfree(&areg);
	munmap(cp_ptr,filesize);
	return 0;
}
