#include"Spider.h"


int Remove_Bloom( char* str,int len)
{
	if(search((unsigned char*)str,len)==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Insert_Bloom( char* str,int len)
{
	insert((unsigned char*)str,len);
	return 0;
}
