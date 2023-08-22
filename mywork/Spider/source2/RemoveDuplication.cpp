#include"Spider.h"


int RemoveDuplication(Container*yct,Container*wct,char* new_link)
{
	int flag;//环形遍历的下标
	//先遍历就绪队列
	flag=yct->rear;
	while(flag%yct->max!=yct->front)
	{
		if((strncmp(yct->url_list[flag].origin_url,new_link,strlen(new_link)))==0)
			return 0;
		flag++;
	}
	//再遍历未就绪队列
	flag=wct->rear;
	while(flag%wct->max!=wct->front)
	{
		if((strncmp(wct->url_list[flag].origin_url,new_link,strlen(new_link)))==0)
			return 0;
		flag++;
	}
	return 1;
}
