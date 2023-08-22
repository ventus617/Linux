#include"Spider.h"


int Container_get(Container*ct,url_t*url)
{
	if(ct->cur==0)
		return -1;
	*url=ct->url_list[ct->rear];
	ct->rear=(ct->rear+1)%ct->max;
	ct->cur--;
	return 0;
}
