#include"Spider.h"

int Create_request(char* request,url_t* url)
{
	bzero(request,4096);
	//请求方式,资源地址url,版本号 这个url可以放存储地址也可以全放
	//Accept:可以接收的内容格式;设置其权重系数q=,可以接受的内容格式;设置其权重系统q=
	//UA浏览器：版本号 (标准X11 系统 兼容性)
	//域名host
	//链接方式
	sprintf(request,"GET %s HTTP/1.1\r\n"\
					"Accept:text/html,application/xhtml+xml;q=0.9,image/webp;q=0.8\r\n"\
					"User-Agent:Mozilla/5.0 (X11 Linux X86_64)\r\n"\
					"Host:%s\r\n"\
					"Connection:close\r\n\r\n"
					,url->origin_url,url->domain);
	printf("[Download] [Step 4][Request Create successfully]\n%s",request);
	return 0;
}
