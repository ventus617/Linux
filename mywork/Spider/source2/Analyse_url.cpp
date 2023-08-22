#include"Spider.h"

// http://pic1.win4000.com/wallpaper/2018-01-16/5a5daf7d70fd1.jpg 

int Analyse_url(url_t* url,map<unsigned int,string>&hostmap)
{
	char* http[]={(char*)"http://",(char*)"https://",NULL};
	int flags;//遍历下标
	int start;//起始位置
	bzero(url->domain,1024);
	bzero(url->sava_path,1024);
	bzero(url->src_name,1024);
	bzero(url->ip,16);
	if(strncmp(url->origin_url,http[0],strlen(http[0]))==0)
	{
		url->type=0;
		url->port=80;
		start=strlen(http[0]);
	}
	else
	{
		url->port=443;
		url->type=1;
		start=strlen(http[1]);
	}
	flags=0;
	int temp=0;
	//取出域名
	for(flags=start;url->origin_url[flags]!='/'&&url->origin_url[flags]!='\0';flags++)
	{
		url->domain[temp]=url->origin_url[flags];
		temp++;
	}
	//得到文件名长度
	int size=0;
	for(flags=strlen(url->origin_url);url->origin_url[flags]!='/';flags--,size++);
	//取出文件名
	temp=0;
	for(flags=strlen(url->origin_url)-size+1;url->origin_url[flags]!='\0';flags++)
	{
		url->src_name[temp]=url->origin_url[flags];
		temp++;
	}
	//取出存储路径
	temp=0;
	for(flags=start+strlen(url->domain);flags<strlen(url->origin_url)-size+1;flags++)
	{
		url->sava_path[temp]=url->origin_url[flags];
		temp++;
	}
	unsigned int key;
	string str;
	for(int i=0;i<strlen(url->domain);i++)
	{
		key+=url->domain[i];
		str+=url->domain[i];
	}
	//dns函数,获得一个地址列表，得到该地址的多个公网ip
	if(hostmap.count(key)==0)
	{
		struct hostent* ent;
		if((ent=gethostbyname(url->domain))==NULL)
		{
			perror("gethostbyname call failed");
			exit(0);
		}
		inet_ntop(AF_INET,ent->h_addr_list[0],url->ip,16);//大端序转字符串
		str=url->ip;
		hostmap[key]=str;
	}
	else
	{
		strcpy(url->ip,hostmap[key].c_str());
		
	}
	printf("[Download] [Step 2] [Analyse Url Successfully,\norigin_url:[%s],\ndomain:[%s],\nsave_path:[%s],\nsrc_name:[%s],\nip:[%s],port:[%d],http:[%d]]\n",url->origin_url,url->domain,url->sava_path,url->src_name,url->ip,url->port,url->type);
	return 0;
}
