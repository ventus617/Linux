#include"Spider.h"


int Remove_Bloom( char* str,int len,unsigned int* bit_array)
{
	if(search((unsigned char*)str,len,bit_array)==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Insert_Bloom( char* str,int len,unsigned int* bit_array)
{
	insert((unsigned char*)str,len,bit_array);
	return 0;
}
