#include"Spider.h"

int Save_result(const char* h1,const char* description)
{
	char result[2000];
	bzero(result,2000);

	const char* start="<html>\r\n<hread>\r\n"\
					   "<meta http-equiv=\"Content-Type\""\
					   " content=\"text/html;charset=utf-8\">\r\n</head>\r\n"\
					   "<table border=\"1\" cellspacing=\"0\" cellpadding=\"0\" width=\"1920\">\r\n"\
					   "<caption>百科百度词条数据集</caption>\r\n"\
					   "<tr><th>索引编号</th><th>词条标记</th><th>词条描述</th></tr>\r\n";

	//tr是行 th是列
	const char* end="</table>\r\n</html>\r\n";

	if(result_num==0)
	{
		write(result_fd,start,strlen(start));
		return 0;
	}
	sprintf(result,"<tr><td>%d</td><td>%s</td><td>%s</td></tr>\r\n",result_num,h1,description);
	write(result_fd,result,strlen(result));
	//td是正常字体居左  th是粗体居中 都是列
	if(result_num==Result_Max)
	{
		write(result_fd,end,strlen(end));
		close(result_fd);
		return 0;
	}
	return 0;
}
