#include"Spider.h"

int Container_set(Container*ct,url_t url)
{
	if(ct->cur==ct->max)
		return -1;
	ct->url_list[ct->front]=url;
	ct->cur++;
	ct->front=(ct->front+1)%ct->max;
	return 0;
}
