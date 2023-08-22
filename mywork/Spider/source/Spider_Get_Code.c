#include"Spider.h"

int Spider_Get_Code(const char* response)
{
	int code;
	char Status_code[100];//匹配code
	bzero(Status_code,100);
	//HTTP/1.1 200 OK
	regex_t reg;
	regmatch_t ma[2];
	regcomp(&reg,"HTTP/1.1 \\([^\r\n]\\+\\?\\)\r\n",0);
	//获取响应码
	if((regexec(&reg,response,2,ma,0))==0)
	{
		snprintf(Status_code,ma[1].rm_eo-ma[1].rm_so+1,"%s",response+ma[1].rm_so);
		sscanf(Status_code,"%d ",&code);//按照格式将字符串转化传出
		regfree(&reg);
		return code;
	}
	regfree(&reg);
	return -1;

}
