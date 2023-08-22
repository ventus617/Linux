#include"Bloom_Filter.h"

unsigned int MyHash(unsigned char* str,int len,unsigned int* bit_array)
{
	unsigned int hash=0;
	for(int i=0;i<len;i++)
	{
		hash=(hash<<5)+hash+str[i];//每一位字符向左偏移5位
	}
	return hash%Bit_Array_Num; //得到对应的下标值
}

void insert(unsigned char*str,int len,unsigned int* bit_array)
{
	for(int i=0;i<Hash_Num;i++)
	{
		unsigned int index=MyHash(str,len,bit_array)+i*len;//索引是hash返回的下标+对应的i×len
		bit_array[index/8]|=1<<(index%8);//位数组对应下标置1
	}
}

int search(unsigned char* str,int len,unsigned int* bit_array)
{
	for(int i=0;i<Hash_Num;i++)
	{
		unsigned int index=MyHash(str,len,bit_array)+i*len;
		if(!(bit_array[index/8]&(1<<(index%8))))//对应的位不是1
		{
			return 0;
		}
	}
	return 1;
}
