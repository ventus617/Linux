//http://pic1.win4000.com/wallpaper/2018-01-16/5a5daf7d70fd1.jpg 
//https://baike.baidu.com/item/%E5%8E%9F%E7%A5%9E/23583622?fr=ge_ala
#include"Spider.h"

int main(int argc,char**argv)
{
	map<unsigned int,string>hostmap;
	//hash位图数组
	unsigned int bit_array[Bit_Array_Num/8];
	memset(bit_array,0,sizeof(bit_array));
	//记录下载数量
	int result_num;
	 //解析开关
	 int analyze_shutdown=1;//0关 1开
	 //结果文件的描述符
	  int result_fd;
	url_t url;//传入种子的节点
	SSL_t* ssl=NULL;
	if(argc!=2)
	{
		printf("Usage: url\n");
		exit(0);
	}
	char* src_url=argv[1];
	//char*src_url="http://pic1.win4000.com/wallpaper/2018-01-16/5a5daf7d70fd1.jpg";
	//char* src_url="https://baike.baidu.com/item/%E5%8E%9F%E7%A5%9E/23583622?fr=ge_ala";
	//char* src_url="https://lmg.jj20.com/up/allimg/tp05/191002011311K06-0-lp.jpg";
	//char* src_url="https://baike.baidu.com/item/%E5%8E%9F%E7%A5%9E/23583622?fr=ge_ala";
	//char* src_url="https://baike.baidu.com/item/ChatGPT/62446358?fr=ge_ala";
	strcpy(url.origin_url,src_url);
	int sockfd;
	//创建就绪队列
	Container* wct=NULL;
	Container* yct=NULL;
	wct=Create_Container(500);
	yct=Create_Container(Result_Max);
	umask(0000);
	//创建结果文件
	result_fd=open("Result.html",O_RDWR|O_CREAT,0775);
	Save_result(NULL,NULL,result_fd,result_num);
	//进行去重校验(可能之前有缓存,这处理和未处理队列都有持久化的数据)
	if(Remove_Bloom(src_url,strlen(src_url),bit_array)==0)
	{
		strcpy(url.origin_url,src_url);
		//放入布隆过滤器
		Insert_Bloom(src_url,strlen(src_url),bit_array);
		Container_set(wct,url);
	}
	analyze_shutdown=1;
	url_t tmpUrl;//传出的节点
	//开始解析爬取
	while(wct->cur>0&&yct->cur<Result_Max)
	{
		Container_get(wct,&tmpUrl);
		printf("%s\n",tmpUrl.origin_url);
		sockfd=TP_Netinit();
		Analyse_url(&tmpUrl,hostmap);
		Connect_Web(&tmpUrl,sockfd);
		if(tmpUrl.type)
		{
			ssl=Spider_Openssl_Init(sockfd);
		}
		char request[4096];
		Create_request(request,&tmpUrl);
		if((Download(&tmpUrl,sockfd,request,ssl))==-1)
		{
			continue;
		}
		Container_set(yct,tmpUrl);//放入已处理队列
		Analyze_html(&tmpUrl,wct,yct,analyze_shutdown,result_num,bit_array,result_fd);
		printf("\n");
	}
	printf("Spider is Done..\n");
	for(int i=0;i<Result_Max;i++)
	{
		printf("%s\n",yct->url_list[i].origin_url);
	}
	return 0;
}
