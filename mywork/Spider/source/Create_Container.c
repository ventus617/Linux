#include"Spider.h"

Container* Create_Container(int num)
{
	Container*ct=(Container*)malloc(sizeof(Container));
	ct->url_list=(url_t*)malloc(sizeof(url_t)*num);
	ct->max=num;
	ct->cur=0;
	ct->front=0;
	ct->rear=0;
	return ct;
}
